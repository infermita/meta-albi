# Base this image on rpi-hwup-image
include recipes-core/images/core-image-base.bb 

IMAGE_FEATURES += "package-management "
IMAGE_ROOTFS_EXTRA_SPACE = "524288"
IMAGE_INSTALL += "\
			kernel-modules \
			openssh \
			usbutils \
			wiringpi \
			linux-firmware \
			tzdata \
			tzdata-europe \
			m53 \
			"
