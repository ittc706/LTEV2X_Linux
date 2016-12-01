

# 开始转码配置文件

mkdir ./Config/Unicode ./Config/UTF-8

for filename in $(ls ./Config/*.xml)
do
	echo "正在转码: "$filename
	native2ascii ./Config/$filename ./Config/Unicode/$filename
	native2ascii -reverse -encoding utf-8 ./Config/Unicode/$filename ./Config/UTF-8/$filename
done

rm -f ./Config/*.xml

mv ./Config/UTF-8/* ./Config

rm -rf ./Config/Unicode ./Config/UTF-8

sleep 1s
