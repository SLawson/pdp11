./macro11 BA_01.mac -O BA_01.obj -l BA_01.lst
./obj2ascii BA_01.obj BA_01.ascii
./macro11 BA_02.mac -O BA_02.obj -l BA_02.lst
./obj2ascii BA_02.obj BA_02.ascii

python3 ./set_address.py
