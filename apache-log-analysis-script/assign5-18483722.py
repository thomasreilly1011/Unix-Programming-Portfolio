#!/usr/bin/python
# Write by Thomas Reilly - 18483722 - thomas.edreilly@gmail.com
# A script that can be used for analysing apache .log files.
# -l [log file name] is compulsory
# -n gives the number of unique IP addresses in the log
# -t [n] gives the top n ip addresses and the number of requests
# -v [ip address] gives the number of visits by the given ip address
# -L [ip address] gives the requests made by the given ip address
# -d [date] where date in the the form DDMMMYYYY e.g. 09Feb2013. Gives the number of requests made by all ips on that
#   date.

import getopt
import re
import sys
import datetime

logs = list()


def read_log(file_path):
    file = open(file_path)
    for line in file:
        logs.append(line)


def get_unique_ips():
    ips = list()
    for line in logs:
        if line is not None:
            ip = re.findall(r'^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}', line)
            if ip:
                ips.append(ip[0])
    ips = list(dict.fromkeys(ips))
    return ips


def top(n):
    n = int(n)
    i = 0
    ips = dict()
    while n != 0:
        line = logs.pop(i)
        i += 1
        if line is not None:
            ip = re.findall(r"\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}", line)
            if ip:
                e = ip[0]
                if e in ips:
                    continue
                else:
                    ips[e] = len(list_requests(e))
                    n -= 1
    return ips


def count_visits(ip):
    ips = list()
    count = 0
    for line in logs:
        if line is not None:
            ips.append(line.split(' ')[0])
    for e in ips:
        if e is ip:
            count += 1
    return count


def get_num_visits(ip):
    visits = 0
    last_datetime = None
    for line in logs:
        if line is not None:
            ips = re.findall(r'\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}', line)
            if ips:
                e = ips[0]
                if e == ip:
                    date_and_time = line.split(" ")[3]
                    date_and_time = date_and_time.replace("[", "")
                    exp = re.match(r'(..)/(...)/(....):(..):(..):(..)', date_and_time)
                    new_datetime = datetime.datetime(int(exp.group(3)), month_abbrv(exp.group(2)), int(exp.group(1)),
                                                     int(exp.group(4)), int(exp.group(5)), int(exp.group(6)))
                    if last_datetime is None:
                        last_datetime = new_datetime
                        visits += 1
                        continue
                    elif (new_datetime - last_datetime) > datetime.timedelta(0, 0, 0, 0, 0, 1, 0):
                        visits += 1
                        last_datetime = new_datetime
                    else:
                        last_datetime = new_datetime
    return visits


def month_abbrv(abbrv):
    return {'Jan': 1, 'Feb': 2, 'Mar': 3, 'Apr': 4, 'May': 5, 'Jun': 6, 'Jul': 7, 'Aug': 8, 'Sep': 9, 'Oct': 10,
            'Nov': 11, 'Dec': 12}[abbrv]


def list_requests(ip):
    requests = list()
    for line in logs:
        if line is not None:
            ips = re.findall(r'\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}', line)
            if ips:
                e = ips[0]
                if e == ip:
                    request = line.replace("\n", "")
                    request = request.split("\"")[1]
                    requests.append(request)
    return requests


def visits_on_date(date):
    formatted_date = date[:2] + "/" + date[2:-4] + "/" + date[5:]
    visits = dict()
    for line in logs:
        if line is not None:
            if formatted_date in line:
                ips = re.findall(r'\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}', line)
                if ips:
                    ip = ips[0]
                    if ip in visits:
                        visits[ip] += 1
                    else:
                        visits[ip] = 1
    return visits


def main(argv):
    opts, args = getopt.getopt(argv, "l:nt:v:L:d:")
    for opt, arg in opts:
        if opt == '-l':
            read_log(arg)
        elif opt == '-n':
            ips = get_unique_ips()
            print("Unique IP Addresses: " + str(len(ips)))
        elif opt == '-t':
            if not isinstance(int(arg), int):
                print("Argument to -t must be a number")
                sys.exit(-1)
            ips = top(arg)
            for ip in ips:
                print(ip + "\t" + str(ips[ip]))
        elif opt == '-v':
            if not re.match(r'\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}', arg):
                print("Argument for -v must be an ip")
            visits = get_num_visits(arg)
            print("Visits by " + arg + ": " + str(visits))
        elif opt == '-L':
            if not re.match(r'\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}', arg):
                print("Argument for -L must be an ip")
            requests = list_requests(arg)
            for request in requests:
                print(request)
        elif opt == '-d':
            if not re.match(r'\d\d\w\w\w\d\d', arg):
                print("Argument for -d must be in form DDMMMYYYY, e.g. 09Feb2013")
            visits = visits_on_date(arg)
            for ip in visits:
                if len(ip) < 12:
                    print(ip + "\t\t\t" + str(visits[ip]))

                else:
                    print(str(ip) + "\t\t" + str(visits[ip]))


main(sys.argv[1:])
