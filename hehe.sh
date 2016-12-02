files=$(find . -regex ".*\.\(h\|cpp\|m\|xml\)")

for file in $files
do
	echo "Convert encoding format: "$file
	native2ascii $file "${file}.tmp"
	rm -f $file
	native2ascii -reverse -encoding utf-8 "${file}.tmp" $file
	rm -f "${file}.tmp"
done