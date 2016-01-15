Name:       dali-addon
Summary:    DALi module for Node.JS
Version:    0.0.1
Release:    1
Group:      Development/Libraries
License:    Apache License, Version 2.0
URL:        https://review.tizen.org/gerrit/#/q/project:platform/core/uifw/dali-toolkit
Distribution: Tizen
Source0:    %{name}-%{version}.tar.gz
Requires:   dali

# Do NOT put an adaptor here - it is an application choice which adaptor to use
BuildRequires:  pkgconfig
BuildRequires:  pkgconfig(dlog)
BuildRequires:  cmake
BuildRequires:  pkgconfig(dali)
BuildRequires:  pkgconfig(dali-core)
BuildRequires:  pkgconfig(dali-toolkit)
BuildRequires:  nodejs-devel

%description
DALi Node.JS addon


##############################
# Define directory locations
# Currently the addon is part of toolkit repository
##############################
# addonDir = node-addon directory inside BUILD directory
%define addonDir %{_builddir}/dali-addon-%{version}/node-addon

# addonBuildDir = node-addon build directory inside BUILD directory
%define addonBuildDir %{addonDir}/build/tizen

# installPath = the directory the node-addon is installed into
# we use /usr/lib/node so the developer can do var dali = require('dali');
%define installPath /usr/lib/node

# full install path, for GBS were are installing into a BUILDROOT
%define installDir %{buildroot}%{installPath}

##############################
# Preparation
##############################
%prep

# setup = cd build area, decompresses source, sets all file permisions to be owned by root ( q = quiet )
%setup -q

##############################
# Build
##############################
%build

echo "----------------Build directory = " %{addonBuildDir}
PREFIX="/usr"
CXXFLAGS+=" -std=c++11 -Wall -g -Os -fPIC -fvisibility-inlines-hidden -fdata-sections -ffunction-sections "
LDFLAGS+=" -Wl,--rpath=$PREFIX/lib -Wl,--as-needed -Wl,--gc-sections -Wl,-Bsymbolic-functions "

cd "%{addonBuildDir}"
cmake -DCMAKE_INSTALL_PREFIX=%{installPath}  -DGBSBuild:BOOL=1 .
make

##############################
# Installation
##############################
%install
rm -rf %{buildroot}

cd "%{addonBuildDir}"
%make_install
cp %{addonDir}/line-mesh.js %{installDir}/line-mesh.js


%clean
rm -rf %{buildroot}

##############################
# Post Install
##############################
%post
exit 0


##############################
# Post Uninstall
##############################
%postun

##############################
# Files in Binary Packages
##############################
%files
%manifest dali-addon.manifest
%defattr(-,root,root,-)
%{installPath}


