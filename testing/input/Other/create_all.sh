./macro11 OT_01.mac -O OT_01.obj -l OT_01.lst
./obj2ascii OT_01.obj OT_01.ascii
./macro11 OT_02.mac -O OT_02.obj -l OT_02.lst
./obj2ascii OT_02.obj OT_02.ascii

python3 ./set_address.py
