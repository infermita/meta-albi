# Base this image on rpi-hwup-image
include recipes-core/images/rpi-hwup-image.bb
IMAGE_FEATURES += "package-management "
#IMAGE_ROOTFS_EXTRA_SPACE = "52428"
IMAGE_INSTALL += "\
			openssh \
			wpa-supplicant \
			wiringpi \
			linux-firmware \
			tzdata \
			tzdata-europe \
			python-broadlink \
			mariadb \
			php \
			apache2 \
			"