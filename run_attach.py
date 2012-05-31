import subprocess
def do():
	LogDir = "/home/e/getProcessFuncCallRelation/Log/"
	cmd = "rm -f " + LogDir+"*"
	print "cmd>>", cmd
	subprocess.call(cmd, shell=True)
	cmd = "ls " + LogDir
	print "cmd>>", cmd
	subprocess.call(cmd, shell=True)
	cmd = "ps -ef | grep ./task"
	print "cmd>>", cmd
	data=subprocess.Popen(cmd,stdout=subprocess.PIPE,shell=True)

	process_info = data.stdout.read()
	print process_info
	print "split>>", process_info.split(' ')
	pid = process_info.split(' ')[9]
	print "pid>>", pid
	
	my_so = "/home/e/getProcessFuncCallRelation/Obj/Main.so"
	pin = "/home/e/pin/pin"
	flag1 = " -pid "
	flag2 = " -t "

	cmd = pin + flag1 + pid + flag2 + my_so
	print "cmd>>", cmd
	subprocess.call(cmd, shell=True)
#sudo /home/e/pin/pin -pid    -t /home/e/Desktop/getProcessFuncCallRelation/Obj/getProcessFuncCallRelation.so
if __name__ == '__main__':
	do()
