./macro11 CC_01.mac -O CC_01.obj -l CC_01.lst
./obj2ascii CC_01.obj CC_01.ascii
./macro11 CC_02.mac -O CC_02.obj -l CC_02.lst
./obj2ascii CC_02.obj CC_02.ascii
./macro11 CC_03.mac -O CC_03.obj -l CC_03.lst
./obj2ascii CC_03.obj CC_03.ascii
./macro11 CC_04.mac -O CC_04.obj -l CC_04.lst
./obj2ascii CC_04.obj CC_04.ascii
./macro11 CC_05.mac -O CC_05.obj -l CC_05.lst
./obj2ascii CC_05.obj CC_05.ascii
./macro11 CC_06.mac -O CC_06.obj -l CC_06.lst
./obj2ascii CC_06.obj CC_06.ascii
./macro11 CC_07.mac -O CC_07.obj -l CC_07.lst
./obj2ascii CC_07.obj CC_07.ascii
./macro11 CC_08.mac -O CC_08.obj -l CC_08.lst
./obj2ascii CC_08.obj CC_08.ascii
./macro11 CC_09.mac -O CC_09.obj -l CC_09.lst
./obj2ascii CC_09.obj CC_09.ascii
./macro11 CC_10.mac -O CC_10.obj -l CC_10.lst
./obj2ascii CC_10.obj CC_10.ascii

python3 ./set_address.py
