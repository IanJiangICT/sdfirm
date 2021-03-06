#!/bin/sh

SCRIPT=`(cd \`dirname $0\`; pwd)`
GEM5_SRC=~/workspace/gem5
GEM5_DBG=Minor
GEM5_IMG=linaro-minimal-aarch64.img
GEM5_KERN=vmlinux.aarch64.20140821
GEM5_DTB=vexpress.aarch64.20140821.dtb
GEM5_CPUS=1
#GEM5_KERN=vmlinux.aarch64.local

usage()
{
	echo "Usage:"
	echo "`basename $0` [-c cpus] [-i] [-m] [-r] [-s gem5] [-u] [-x]"
	echo "Where:"
	echo " -c num-cpus: specify number of CPUs"
	echo " -i:          specify interrupt debugging"
	echo " -m:          specify memory debugging"
	echo " -r:          specify register debugging"
	echo " -s gem5-dir: specify GEM5 source directory"
	echo " -u:          use aarch64-ubuntu-trusty-headless.img"
	echo " -x:          specify execution debugging"
	exit $1
}

fatal_usage()
{
	echo $1
	usage 1
}

while getopts "c:imrs:ux" opt
do
	case $opt in
	c) GEM5_CPUS=$OPTARG;;
	i) GEM5_DBG=${GEM5_DBG},Faults,GIC;;
	m) GEM5_DBG=${GEM5_DBG},AddrRanges;;
	r) GEM5_DBG=${GEM5_DBG},MiscRegs;;
	x) GEM5_DBG=${GEM5_DBG},Exec;;
	s) GEM5_SRC=$OPTARG;;
	u) GEM5_IMG=aarch64-ubuntu-trusty-headless.img;;
	?) echo "Invalid argument $opt"
	   fatal_usage;;
	esac
done
shift $(($OPTIND - 1))

SDFIRM_SRC=`(cd ${SCRIPT}/..; pwd)`
GEM5_SRC=`(cd ${GEM5_SRC}; pwd)`

if [ ! -f "${GEM5_SRC}/fs_images/arm/binaries/boot_emm.arm64.orig" ]; then
	echo "Backing up original boot_emm.arm64..."
	mv -i \
	${GEM5_SRC}/fs_images/arm/binaries/boot_emm.arm64 \
	${GEM5_SRC}/fs_images/arm/binaries/boot_emm.arm64.orig
fi
cp ${SDFIRM_SRC}/sdfirm.strip ${GEM5_SRC}/fs_images/arm/binaries/boot_emm.arm64

(
	cd ${GEM5_SRC}
	export M5_PATH="${GEM5_SRC}/fs_images/arm/"

	rm -f ${GEM5_SRC}/m5out/system.terminal
	./build/ARM/gem5.opt --debug-flags=${GEM5_DBG} \
		configs/example/fs.py \
		--num-cpus=${GEM5_CPUS} \
		--kernel=${GEM5_KERN} \
		--machine-type=VExpress_EMM64 \
		--dtb-file=${GEM5_DTB} \
		--disk-image=${GEM5_IMG}
	cat ${GEM5_SRC}/m5out/system.terminal
)
