# Base this image on rpi-hwup-image
include recipes-core/images/core-image-base.bb
IMAGE_FEATURES += "package-management "
#IMAGE_ROOTFS_EXTRA_SPACE = "52428"
IMAGE_INSTALL += "\
			kernel-modules \
			openssh \
			libnfc \
			wpa-supplicant \
			usbutils \
			wiringpi \
			linux-firmware \
			qtbase \
			alucount \
			vc-graphics-hardfp \
			wireless-tools \
			tzdata \
			tzdata-europe \
			i2c-tools \
			gdb \
			ntpclient \
			e2fsprogs-ptest \
			e2fsprogs-e2fsck \
			"
