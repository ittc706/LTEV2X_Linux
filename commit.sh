if [ ! -d ../LTEV2X/LTEV2X ];then
	echo "Please place LTEV2X and LTEV2X_Linux in the same directory"
	exit
fi

# First obtain the LTEV2X's git version number

cd ../LTEV2X/LTEV2X/
uq=$(git rev-list --all | head -n 1)
uq=${uq:0:7}

# Write the format version information

description="## version_"$(date +%Y)"_"$(date +%m)"_"$(date +%d)"("$uq")"

cd ../../LTEV2X_Linux

echo $description >> README.md

# Remove the old files
rm -f *.h *.cpp
rm -rf Config Log WT

sleep 2s

# Copy files from LTEV2X
cp -a ../LTEV2X/LTEV2X/*.cpp  ../LTEV2X/LTEV2X/*.h  .
cp -a ../LTEV2X/LTEV2X/Config ../LTEV2X/LTEV2X/Log ../LTEV2X/LTEV2X/WT .

sleep 2s

# Begin converting the encoding format

files=$(find . -regex ".*\.\(h\|cpp\|m\|xml\)")

for file in $files
do
	echo "Convert encoding format: "$file
	native2ascii $file "${file}.tmp"
	rm -f $file
	native2ascii -reverse -encoding utf-8 "${file}.tmp" $file
	rm -f "${file}.tmp"
done

# Commit

git add .

git commit -m ${description:3}

git push origin master:master