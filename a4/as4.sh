read -p "Please enter your password:" password

echo "echo $password" > ./password
chmod 777 ./password

echo '
export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh april
' > ./n16.sh
chmod 777 ./n16.sh
./n16.sh