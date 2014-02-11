./macro11 SR_01.mac -O SR_01.obj -l SR_01.lst
./obj2ascii SR_01.obj SR_01.ascii
./macro11 SR_02.mac -O SR_02.obj -l SR_02.lst
./obj2ascii SR_02.obj SR_02.ascii
./macro11 SR_03.mac -O SR_03.obj -l SR_03.lst
./obj2ascii SR_03.obj SR_03.ascii

python3 ./set_address.py
