# This is mqtt_program Makefile
include $(TOPDIR)/rules.mk

PKG_NAME:=openvpnmanagement
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

include $(INCLUDE_DIR)/package.mk

define Package/openvpnmanagement
	CATEGORY:=Base system
	TITLE:=openvpnmanagement
	DEPENDS:=+libubus +libubox +libuci +libblobmsg-json
endef

define Package/openvpnmanagement/description
	This is a mqtt app package
endef

define Package/openvpnmanagement/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/openvpnmanagement $(1)/usr/bin
endef

$(eval $(call BuildPackage,openvpnmanagement, +libcurl))