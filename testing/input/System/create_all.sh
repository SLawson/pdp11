./macro11 SY_01.mac -O SY_01.obj -l SY_01.lst
./obj2ascii SY_01.obj SY_01.ascii

python3 ./set_address.py
