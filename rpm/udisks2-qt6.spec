Name:           udisks2-qt6
Version:        6.0.0
Release:        3%{?dist}
Summary:        Qt6 binding for udisks2
License:        GPLv3+
URL:            https://github.com/linuxdeepin/udisks2-qt6
Source0:        %{name}_%{version}.tar.gz
BuildRequires:  gcc-c++
BuildRequires:  pkgconfig(Qt6Core)
BuildRequires:  pkgconfig(Qt6DBus)
BuildRequires:  cmake

%description
This package provides a Qt6 binding for udisks2.

%package devel
Summary:        Development package for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       qt6-qtbase-devel%{?isa}
Requires:       qt6-qtdbus-devel%{?isa}

%description devel
Header files and libraries for %{name}.

%prep
%setup -q

%build
%cmake -DCMAKE_INSTALL_PREFIX=%{_prefix} \
       -DCMAKE_BUILD_TYPE=Release
%cmake_build

%install
%cmake_install

%files
%doc CHANGELOG.md
%{_libdir}/libudisks2-qt6.so.*

%files devel
%{_includedir}/udisks2-qt6/
%{_libdir}/cmake/udisks2-qt6/
%{_libdir}/pkgconfig/udisks2-qt6.pc
%{_libdir}/libudisks2-qt6.so

%changelog
* Wed Jul 29 2020 Fedora Release Engineering <releng@fedoraproject.org> - 5.0.0-3
- Rebuilt for https://fedoraproject.org/wiki/Fedora_33_Mass_Rebuild 