# Base this image on rpi-hwup-image
include recipes-core/images/rpi-hwup-image.bb
IMAGE_FEATURES += "package-management "
#IMAGE_ROOTFS_EXTRA_SPACE = "52428"
IMAGE_INSTALL += "\
			openssh \
			libnfc \
			wpa-supplicant \
			usbutils \
			wiringpi \
			linux-firmware \
			qtbase \
			alubadge \
			vc-graphics-hardfp \
			wireless-tools \
			tzdata \
			tzdata-europe \
			i2c-tools \
			ntpclient \
			"
