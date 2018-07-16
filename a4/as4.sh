#!/bin/bash
MCA1="224.5.5.5"
P1="5678"

join_time=1
start_sending_time=$((join_time+1))
sending_duration=60
hz_of_sending_packet="0.5"
time_for_one_packet=2
packet_count=$((sending_duration/time_for_one_packet))
leave_time=$((start_sending_time+sending_duration+2))

read -p "Please enter your password:" password
echo "echo $password" > ./password
chmod 777 ./password


echo "echo 'yes'" > ./ECDSA_Pass
chmod 777 ./ECDSA_Pass

#Step 0 for capturing

#Choose April as PCnet16
echo "
export SSH_ASKPASS='./ECDSA_Pass'
setsid ssh april 'exit;'



export SSH_ASKPASS='./password'
export DISPLAY=YOURDOINGITWRONG
setsid ssh april 'tshark -i eth1 -a duration:$((leave_time+3)) -w PCnet16.pcap;exit;'

export SSH_ASKPASS='./password'
setsid sftp april << !
 get PCnet16.pcap
 quit
!

# export SSH_ASKPASS='./password'
# export DISPLAY=YOURDOINGITWRONG
# setsid ssh april 'rm PCnet16.pcap;exit;'


echo 'PCnet16.sh ends!'

" > ./PCnet16.sh
chmod 777 ./PCnet16.sh



#Choose November as PCnet17
echo "

export SSH_ASKPASS='./ECDSA_Pass'
setsid ssh november 'exit;'

export SSH_ASKPASS='./password'
export DISPLAY=YOURDOINGITWRONG
setsid ssh november 'tshark -i eth1 -a duration:$((leave_time+3)) -w PCnet17.pcap;exit;'

export SSH_ASKPASS='./password'
setsid sftp november << !
 get PCnet17.pcap
 quit
!

# export SSH_ASKPASS='./password'
# export DISPLAY=YOURDOINGITWRONG
# setsid ssh april 'rm PCnet16.pcap;exit;'


echo 'PCnet17.sh ends!'

" > ./PCnet17.sh
chmod 777 ./PCnet17.sh


#Choose July as PCnet18
echo "

export SSH_ASKPASS='./ECDSA_Pass'
setsid ssh july 'exit;'

export SSH_ASKPASS='./password'
export DISPLAY=YOURDOINGITWRONG
setsid ssh july 'tshark -i eth1 -a duration:$((leave_time+3)) -w PCnet18.pcap;exit;'

export SSH_ASKPASS='./password'
setsid sftp july << !
 get PCnet18.pcap
 quit
!

# export SSH_ASKPASS='./password'
# export DISPLAY=YOURDOINGITWRONG
# setsid ssh april 'rm PCnet16.pcap;exit;'


echo 'PCnet18.sh ends!'

" > ./PCnet18.sh
chmod 777 ./PCnet18.sh


#Choose October as PCnet19
echo "
export SSH_ASKPASS='./ECDSA_Pass'
setsid ssh october 'exit;'


export SSH_ASKPASS='./password'
export DISPLAY=YOURDOINGITWRONG
setsid ssh october 'tshark -i eth1 -a duration:$((leave_time+3)) -w PCnet19.pcap;exit;'

export SSH_ASKPASS='./password'
setsid sftp october << !
 get PCnet19.pcap
 quit
!

# export SSH_ASKPASS='./password'
# export DISPLAY=YOURDOINGITWRONG
# setsid ssh april 'rm PCnet16.pcap;exit;'

echo 'PCnet19.sh ends!'


" > ./PCnet19.sh
chmod 777 ./PCnet19.sh









#Choose June as MCRnet16

echo "
$join_time JOIN $MCA1 PORT $P1
$leave_time LEAVE $MCA1 PORT $P1

" > ./MCRnet.mgn
chmod 777 ./MCRnet.mgn

echo '

export SSH_ASKPASS="./ECDSA_Pass"
setsid ssh june "exit;"

export SSH_ASKPASS="./password"
setsid sftp june << !
 put MCRnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh june "mgen input MCRnet.mgn;rm MCRnet.mgn;exit;"

echo "MCR1net16.sh ends!"
' > ./MCR1net16.sh
chmod 777 ./MCR1net16.sh

#===
#Choose May as MCRnet18

echo '

export SSH_ASKPASS="./ECDSA_Pass"
setsid ssh may "exit;"

export SSH_ASKPASS="./password"
setsid sftp may << !
 put MCRnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh may "mgen input MCRnet.mgn;rm MCRnet.mgn;exit;"

echo "MCR1net18.sh ends!"
' > ./MCR1net18.sh
chmod 777 ./MCR1net18.sh

#====
#Choose year as MCRnet19

echo '

export SSH_ASKPASS="./ECDSA_Pass"
setsid ssh year "exit;"

export SSH_ASKPASS="./password"
setsid sftp year << !
 put MCRnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh year "mgen input MCRnet.mgn;rm MCRnet.mgn;exit;"

echo "MCR1net19.sh ends!"
' > ./MCR1net19.sh
chmod 777 ./MCR1net19.sh
#====
#Choose August as MCR1net17

echo '

export SSH_ASKPASS="./ECDSA_Pass"
setsid ssh august "exit;"

export SSH_ASKPASS="./password"
setsid sftp august << !
 put MCRnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh august "mgen input MCRnet.mgn;rm MCRnet.mgn;exit;"

echo "MCR1net17.sh ends!"
' > ./MCR1net17.sh
chmod 777 ./MCR1net17.sh

#===
#Choose Spring as MCR2net17

echo '

export SSH_ASKPASS="./ECDSA_Pass"
setsid ssh spring "exit;"

export SSH_ASKPASS="./password"
setsid sftp spring << !
 put MCRnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh spring "mgen input MCRnet.mgn;rm MCRnet.mgn;exit;"
echo "MCR2net17.sh ends!"
' > ./MCR2net17.sh
chmod 777 ./MCR2net17.sh
#=================================================================================
















#Choose September as MCSnet16

echo "
$start_sending_time ON 1 UDP SRC $P1 DST $MCA1/$P1 PERIODIC [$hz_of_sending_packet 512] COUNT $packet_count

" > ./MCSnet.mgn
chmod 777 ./MCSnet.mgn



echo '

export SSH_ASKPASS="./ECDSA_Pass"
setsid ssh september "exit;"

export SSH_ASKPASS="./password"
setsid sftp september << !
 put MCSnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh september "mgen input MCSnet.mgn;rm MCSnet.mgn;exit;"

echo "MCSnet16.sh ends!"
' > ./MCSnet16.sh
chmod 777 ./MCSnet16.sh


#Choose Autumn as MCSnet17



echo '

export SSH_ASKPASS="./ECDSA_Pass"
setsid ssh autumn "exit;"

export SSH_ASKPASS="./password"
setsid sftp autumn << !
 put MCSnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh autumn "mgen input MCSnet.mgn;rm MCSnet.mgn;exit;"

echo "MCSnet17.sh ends!"
' > ./MCSnet17.sh
chmod 777 ./MCSnet17.sh

#Choose Winter as MCSnet18



echo '

export SSH_ASKPASS="./ECDSA_Pass"
setsid ssh winter "exit;"

export SSH_ASKPASS="./password"
setsid sftp winter << !
 put MCSnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh winter "mgen input MCSnet.mgn;rm MCSnet.mgn;exit;"

echo "MCSnet18.sh ends!"
' > ./MCSnet18.sh
chmod 777 ./MCSnet18.sh


#Choose Solstice as MCSnet19



echo '

export SSH_ASKPASS="./ECDSA_Pass"
setsid ssh solstice "exit;"

export SSH_ASKPASS="./password"
setsid sftp solstice << !
 put MCSnet.mgn
 quit
!

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh solstice "mgen input MCSnet.mgn;rm MCSnet.mgn;exit;"

echo "MCSnet19.sh ends!"
' > ./MCSnet19.sh
chmod 777 ./MCSnet19.sh








./PCnet16.sh &
./MCR1net16.sh &
./MCSnet16.sh &

./PCnet17.sh &
./MCR1net17.sh &
./MCR2net17.sh &
./MCSnet17.sh &

./PCnet18.sh &
./MCR1net18.sh &
./MCSnet18.sh &


./PCnet19.sh &
./MCR1net19.sh &
./MCSnet19.sh &




for pids in `jobs -p`
do
echo $pids
    wait $pids
done




rm ./PCnet16.sh
rm ./MCR1net16.sh
rm ./MCSnet16.sh
rm ./password
rm ./PCnet17.sh 
rm ./MCR1net17.sh 
rm ./MCR2net17.sh
rm ./MCSnet17.sh 

rm ./PCnet18.sh 
rm ./MCR1net18.sh
rm ./MCSnet18.sh


rm ./PCnet19.sh
rm ./MCR1net19.sh
rm ./MCSnet19.sh

echo "Finished!"