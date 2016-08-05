1.进入bak文件夹，文件夹内有两个文件 chrdrvtest1.c 和 Makefile 。
2.执行make。
3.ls查看生成的文件。
4.找chrdrvtest1.ko这个模块。
5.插入模块，root权限下执行，sudo insmod chrdrvtest1.ko
6.查看内核消息 直接执行dmesg ，或者 cat /proc/kmsg 
	内核信息会打印“*****enter chrdrvtest_init*****”
7.卸载模块，同样要root权限，sudo rmmod chrdrvtest1
8.查看内核信息，同第6步。

9.测试完成后，执行make clean。

NOTE:此测试是刚开始最原始版本，现在chrdrvtest.c文件已经更新。
