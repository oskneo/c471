#!/bin/bash
MCA1="224.5.5.5"
P1="5678"

join_time=1
start_sending_time=$((join_time+1))
sending_duration=10
hz_of_sending_packet="0.5"
time_for_one_packet=2
packet_count=$((sending_duration/time_for_one_packet))
leave_time=$((start_sending_time+sending_duration+2))

read -p "Please enter your password:" password
echo "echo $password" > ./password
chmod 777 ./password

#Step 0 for capturing

#Choose April as PCnet16
echo "
export SSH_ASKPASS='./password'
export DISPLAY=YOURDOINGITWRONG
setsid ssh april 'tshark -i eth1 -a duration:$sending_duration -w n16.pcap;exit;'

export SSH_ASKPASS='./password'
setsid sftp april << !
 get n16.pcap
 quit
!

# export SSH_ASKPASS='./password'
# export DISPLAY=YOURDOINGITWRONG
# setsid ssh april 'rm n16.pcap;exit;'




" > ./n16pc.sh
chmod 777 ./n16pc.sh



#Choose November as PCnet17
echo "
export SSH_ASKPASS='./password'
export DISPLAY=YOURDOINGITWRONG
setsid ssh november 'tshark -i eth1 -a duration:$sending_duration -w n17.pcap;exit;'

export SSH_ASKPASS='./password'
setsid sftp november << !
 get n17.pcap
 quit
!

# export SSH_ASKPASS='./password'
# export DISPLAY=YOURDOINGITWRONG
# setsid ssh april 'rm n16.pcap;exit;'




" > ./n17pc.sh
chmod 777 ./n17pc.sh


#Choose July as PCnet18
echo "
export SSH_ASKPASS='./password'
export DISPLAY=YOURDOINGITWRONG
setsid ssh july 'tshark -i eth1 -a duration:$sending_duration -w n18.pcap;exit;'

export SSH_ASKPASS='./password'
setsid sftp july << !
 get n18.pcap
 quit
!

# export SSH_ASKPASS='./password'
# export DISPLAY=YOURDOINGITWRONG
# setsid ssh april 'rm n16.pcap;exit;'




" > ./n18pc.sh
chmod 777 ./n18pc.sh


#Choose October as PCnet19
echo "
export SSH_ASKPASS='./password'
export DISPLAY=YOURDOINGITWRONG
setsid ssh october 'tshark -i eth1 -a duration:$sending_duration -w n19.pcap;exit;'

export SSH_ASKPASS='./password'
setsid sftp october << !
 get n19.pcap
 quit
!

# export SSH_ASKPASS='./password'
# export DISPLAY=YOURDOINGITWRONG
# setsid ssh april 'rm n16.pcap;exit;'




" > ./n19pc.sh
chmod 777 ./n19pc.sh









#Choose June as MCRnet16

echo "
$join_time JOIN $MCA1 PORT $P1
$leave_time LEAVE $MCA1 PORT $P1

" > ./MCRnet.mgn
chmod 777 ./MCRnet.mgn

echo '
export SSH_ASKPASS="./password"
setsid sftp june << !
 put MCRnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh june "mgen input MCRnet.mgn;rm MCRnet.mgn;exit;"
' > ./n16mcr.sh
chmod 777 ./n16mcr.sh

#===
#Choose May as MCRnet18

echo '
export SSH_ASKPASS="./password"
setsid sftp may << !
 put MCRnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh may "mgen input MCRnet.mgn;rm MCRnet.mgn;exit;"
' > ./n18mcr.sh
chmod 777 ./n18mcr.sh

#====
#Choose year as MCRnet19

echo '
export SSH_ASKPASS="./password"
setsid sftp year << !
 put MCRnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh year "mgen input MCRnet.mgn;rm MCRnet.mgn;exit;"
' > ./n19mcr.sh
chmod 777 ./n19mcr.sh
#====
#Choose August as MCR1net17

echo '
export SSH_ASKPASS="./password"
setsid sftp august << !
 put MCRnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh august "mgen input MCRnet.mgn;rm MCRnet.mgn;exit;"
' > ./n17mcr.sh
chmod 777 ./n17mcr.sh

#===
#Choose Spring as MCR2net17

echo '
export SSH_ASKPASS="./password"
setsid sftp spring << !
 put MCRnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh spring "mgen input MCRnet.mgn;rm MCRnet.mgn;exit;"
' > ./n17_2mcr.sh
chmod 777 ./n17_2mcr.sh
#=================================================================================
















#Choose September as MCSnet16

echo "
$start_sending_time ON 1 UDP SRC $P1 DST $MCA1/$P1 PERIODIC [$hz_of_sending_packet 512] COUNT $packet_count

" > ./MCSnet.mgn
chmod 777 ./MCSnet.mgn



echo '
export SSH_ASKPASS="./password"
setsid sftp september << !
 put MCSnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh september "mgen input MCSnet.mgn;rm MCSnet.mgn;exit;"
' > ./n16mcs.sh
chmod 777 ./n16mcs.sh


#Choose Autumn as MCSnet17



echo '
export SSH_ASKPASS="./password"
setsid sftp autumn << !
 put MCSnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh autumn "mgen input MCSnet.mgn;rm MCSnet.mgn;exit;"
' > ./n17mcs.sh
chmod 777 ./n17mcs.sh

#Choose Winter as MCSnet18



echo '
export SSH_ASKPASS="./password"
setsid sftp winter << !
 put MCSnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh winter "mgen input MCSnet.mgn;rm MCSnet.mgn;exit;"
' > ./n18mcs.sh
chmod 777 ./n18mcs.sh


#Choose Solstice as MCSnet19



echo '
export SSH_ASKPASS="./password"
setsid sftp solstice << !
 put MCSnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh solstice "mgen input MCSnet.mgn;rm MCSnet.mgn;exit;"
' > ./n19mcs.sh
chmod 777 ./n19mcs.sh








./n16pc.sh &
./n16mcr.sh &
./n16mcs.sh &

./n17pc.sh &
./n17mcr.sh &
./n17_2mcr.sh &
./n17mcs.sh &

./n18pc.sh &
./n18mcr.sh &
./n18mcs.sh &


./n19pc.sh &
./n19mcr.sh &
./n19mcs.sh &




for pids in `jobs -p`
do
echo $pids
    wait $pids
done




rm ./n16pc.sh
rm ./n16mcr.sh
rm ./n16mcs.sh
rm ./password

echo "Finished!"