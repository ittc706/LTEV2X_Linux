
cd ../LTEV2X/LTEV2X/
uq=$(git rev-list --all | head -n 1)
uq=${uq:0:7}



description="## version_"$(date +%Y)"_"$(date +%m)"_"$(date +%d)"("$uq")"

cd ../../LTEV2X_Linux

echo $description >> README.md

# 删除当前的文件
rm -f *.h *.cpp

sleep 2s

# 从对应的目录拷贝过来
cp -a ../LTEV2X/LTEV2X/*.cpp  ../LTEV2X/LTEV2X/*.h  .

sleep 2s

git add .
git commit -m ${description:3}

git push origin master:master