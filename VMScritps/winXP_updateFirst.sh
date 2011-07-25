#!/bin/bash
cd c:
cd /home/bryanta/
cd amos/
./bootstrap > /cygdrive/c/cygwin/winXP_updateFirst.log
if [ $? -ne 0 ]
then
cp /cygdrive/c/cygwin/winXP_updateFirst.log /cygdrive/c/cygwin/winXP_updateFirst_Failed.log
echo "FAILED: ./bootstrap" >> /cygdrive/c/cygwin/winXP_updateFirst_Failed.log
/usr/bin/expect <<EOD
spawn scp /cygdrive/c/cygwin/winXP_updateFirst_Failed.log ssh@sauron.cs.umd.edu:VMlogs
expect "ssh@sauron.cs.umd.edu's password:"
send "123\r"
expect eof
EOD
shutdown /s
fi

./configure --prefix=/usr/local/AMOS >> /cygdrive/c/cygwin/winXP_updateFirst.log
if [ $? -ne 0 ]
then
cp /cygdrive/c/cygwin/winXP_updateFirst.log /cygdrive/c/cygwin/winXP_updateFirst_Failed.log
echo "FAILED: ./configure" >> /cygdrive/c/cygwin/winXP_updateFirst_Failed.log
/usr/bin/expect <<EOD
spawn scp /cygdrive/c/cygwin/winXP_updateFirst_Failed.log ssh@sauron.cs.umd.edu:VMlogs
expect "ssh@sauron.cs.umd.edu's password:"
send "123\r"
expect eof
EOD
shutdown /s
fi

make >> /cygdrive/c/cygwin/winXP_updateFirst.log 
if [ $? -ne 0 ]
then
cp /cygdrive/c/cygwin/winXP_updateFirst.log /cygdrive/c/cygwin/winXP_updateFirst_Failed.log
echo "FAILED: make" >> /cygdrive/c/cygwin/winXP_updateFirst_Failed.log
/usr/bin/expect <<EOD
spawn scp /cygdrive/c/cygwin/winXP_updateFirst_Failed.log ssh@sauron.cs.umd.edu:VMlogs
expect "ssh@sauron.cs.umd.edu's password:"
send "123\r"
expect eof
EOD
shutdown /s
fi

make install >> /cygdrive/c/cygwin/winXP_updateFirst.log
if [ $? -ne 0 ]
then
cp /cygdrive/c/cygwin/winXP_updateFirst.log /cygdrive/c/cygwin/winXP_updateFirst_Failed.log
echo "FAILED: make install" >> /cygdrive/c/cygwin/winXP_updateFirst_Failed.log
/usr/bin/expect <<EOD
spawn scp /cygdrive/c/cygwin/winXP_updateFirst_Failed.log ssh@sauron.cs.umd.edu:VMlogs
expect "ssh@sauron.cs.umd.edu's password:"
send "123\r"
expect eof
EOD
shutdown /s
fi
now=$(date +"%y%m%d")
echo "SUCCESS: complete log stored on http://sauron.cs.umd.edu/$now" >> /cygdrive/c/cygwin/winXP_updateFirst.log
/usr/bin/expect <<EOD
spawn scp /cygdrive/c/cygwin/winXP_updateFirst.log ssh@sauron.cs.umd.edu:VMlogs
expect "ssh@sauron.cs.umd.edu's password:"
send "123\r"
expect eof
EOD
rm /cygdrive/c/cygwin/winXP_updateFirst.log
shutdown /s

