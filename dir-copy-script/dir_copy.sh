# Written by Thomas Reilly - 18483722 - thomas.reilly@ucdconnect.ie
# A bash script that copys the directory structure of the input directory, into a new output directory.
# Any .png files in the input are also transferred into the input and are converted to .jpg.
# The script can be run with the following commands.
# chmod u+x assign4_18483722.sh
# ./assign4_18483722.sh source_directory destination_directory.
#!/bin/bash

# Check number of arguments.
if [ $# -ne 2 ]
then
    echo Incorrect usage.
    echo Correct usage: $0 src dest
    exit
fi

#Check source file exits.
if [ ! -e $1 ]
then
    echo Error. $1 does not exist.
    exit
fi

# Copy over the input ($1), renaming it as the desired output ($2)
cp -r $1 $2
# Check that writing works.
if [ "$?" -ne "0" ]
then
    echo Error, Cannot write to $2
    exit
fi

# Remove any non .png files.
find $2 -type f -not -name "*.png" -exec rm {} + 
# Convert any .png's to .jpg.
find $2 -name "*.png" -exec convert \{} {}.jpg \;
# Remove the no longer nessesary .png extension from any .png.jpg files.
find $2 -name "*.png.jpg" -print0 | while read -d $'\0' f; do mv "${f}" "${f%.png.jpg}.jpg"; done
# Remove the old .png files, no longer needed.
find $2 -name "*.png" -exec rm {} + 
