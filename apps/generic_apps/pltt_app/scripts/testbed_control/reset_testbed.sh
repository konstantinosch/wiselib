echo "...restarting iwsn server on base1.tcs.unige.ch"
sshpass -p 'BASEPASS01' ssh basestation01@base1.tcs.unige.ch ./tr/tr.assembly/bin/tr.iwsn-linux-x86-32 restart;
echo "...restarting iwsn server on base2.tcs.unige.ch"
sshpass -p 'BASEPASS01' ssh basestation01@base2.tcs.unige.ch ./tr/tr.assembly/bin/tr.iwsn-linux-x86-32 restart;
echo "...restarting iwsn server on base3.tcs.unige.ch"
sshpass -p 'BASEPASS01' ssh basestation01@base3.tcs.unige.ch ./tr/tr.assembly/bin/tr.iwsn-linux-x86-32 restart
echo "...restarting iwsn server on base4.tcs.unige.ch"
sshpass -p 'BASEPASS01' ssh basestation01@base4.tcs.unige.ch ./tr/tr.assembly/bin/tr.iwsn-linux-x86-32 restart;
echo "...restarting iwsn server on base5.tcs.unige.ch"
sshpass -p 'BASEPASS01' ssh basestation01@base5.tcs.unige.ch ./tr/tr.assembly/bin/tr.iwsn-linux-x86-32 restart;
echo "...restarting rs server on testbed.tcs.unige.ch"
sshpass -p 'frontspassword' ssh konstantinos@testbed.tcs.unige.ch ./tr/tr.assembly/bin/tr.rs-linux-x86-32 restart;
echo "...restarting iwsn server on testbed.tcs.unige.ch"
sshpass -p 'frontspassword' ssh konstantinos@testbed.tcs.unige.ch ./tr/tr.assembly/bin/tr.iwsn-linux-x86-32 restart;
echo "...restarting snaa server on testbed.tcs.unige.ch"
sshpass -p 'frontspassword' ssh konstantinos@testbed.tcs.unige.ch ./tr/tr.assembly/bin/tr.snaa-linux-x86-32 restart;
