read -p "Please enter your password:" password

echo "echo $password" > ./password
chmod 777 ./password

export SSH_ASKPASS="./password"
export DISPLAY=YOURDOINGITWRONG
setsid ssh april
