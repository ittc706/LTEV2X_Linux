if [ ! -d ../LTEV2X/LTEV2X ];then
	echo "请将LTEV2X与LTEV2X_Linux置于同一目录下"
	exit
fi

cd ../LTEV2X/LTEV2X/
uq=$(git rev-list --all | head -n 1)
uq=${uq:0:7}



description="## version_"$(date +%Y)"_"$(date +%m)"_"$(date +%d)"("$uq")"

cd ../../LTEV2X_Linux

echo $description >> README.md

# 删除当前的文件
rm -f *.h *.cpp
rm -rf Config Log WT

sleep 1s

# 从对应的目录拷贝过来
cp -a ../LTEV2X/LTEV2X/*.cpp  ../LTEV2X/LTEV2X/*.h  .
cp -a ../LTEV2X/LTEV2X/Config ../LTEV2X/LTEV2X/Log ../LTEV2X/LTEV2X/WT .

sleep 1s


mkdir ./Unicode ./UTF-8

# 开始转码.h以及.cpp文件
for filename in $(ls *.h *.cpp)
do
	echo "正在转码: "$filename
	native2ascii $filename ./Unicode/$filename
	native2ascii -reverse -encoding utf-8 ./Unicode/$filename ./UTF-8/$filename
done

rm -f *.h *.cpp

mv ./UTF-8/* .

rm -rf ./Unicode ./UTF-8

sleep 1s

# 开始转码配置文件

mkdir ./Config/Unicode ./Config/UTF-8

for filename in $(ls ./Config/*.xml)
do
	echo "正在转码: "Config/$filename
	native2ascii ./Config/$filename ./Config/Unicode/$filename
	native2ascii -reverse -encoding utf-8 ./Config/Unicode/$filename ./Config/UTF-8/$filename
done

rm -f ./Config/*.xml

mv ./Config/UTF-8/* ./Config

rm -rf ./Config/Unicode ./Config/UTF-8

sleep 1s

git add .
git commit -m ${description:3}

git push origin master:master