#!/bin/bash
MCA1="224.5.5.5"
P1="5678"

read -p "Please enter your password:" password

echo "echo $password" > ./password
chmod 777 ./password


#Choose April as PCnet16
echo '
export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh april "tshark -i eth1 -a duration:15 -w n16.pcap;exit;"

export SSH_ASKPASS="./password"
setsid sftp april << !
 get n16.pcap
 quit
!

# export SSH_ASKPASS="./password"
# export DISPLAY=YOURDOINGITWRONG
# setsid ssh april "rm n16.pcap;exit;"




' > ./n16pc.sh
chmod 777 ./n16pc.sh


#Choose June as MCRnet16

echo "
0.0 JOIN $MCA1 PORT $P1
10.0 LEAVE $MCA1 PORT $P1

" > ./MCRnet16.mgn
chmod 777 ./MCRnet16.mgn



echo '
export SSH_ASKPASS="./password"
setsid sftp june << !
 put MCRnet16.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh june "mgen input MCRnet16.mgn;rm MCRnet16.mgn;exit;"
' > ./n16mcr.sh
chmod 777 ./n16mcr.sh



#Choose September as MCSnet16

echo "
2.0 ON 1 UDP SRC $P1 DST $MCA1/$P1 PERIODIC [0.5 512] COUNT 3

" > ./MCSnet16.mgn
chmod 777 ./MCSnet16.mgn



echo '
export SSH_ASKPASS="./password"
setsid sftp september << !
 put MCSnet16.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh september "mgen input MCSnet16.mgn;rm MCRnet16.mgn;exit;"
' > ./n16mcs.sh
chmod 777 ./n16mcs.sh






./n16pc.sh &
./n16mcr.sh &
./n16mcs.sh &




rm ./n16pc.sh
rm ./n16mcr.sh
rm ./n16mcs.sh
rm ./password

echo "Finished!\n"