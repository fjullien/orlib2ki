package provide capMenuUtil 1.0
namespace eval ::capMenuUtil {
}

proc ::capMenuUtil::addLibXMLMenu { } {
	AddAccessoryMenu "Libs" "Update XML libraries" "::capMenuUtil::update"
	AddAccessoryMenu "Libs" "Convert XML libraries to KiCAD" "::capMenuUtil::convert"
}

proc ::capMenuUtil::update { pLib } {
	set olbLibPath "M:/Developpement/08_Gest_Comp_Lib"
	set orlib2ki "E:/orlib2ki/orlib2ki.exe"
	set allLibs [glob -directory $olbLibPath -- "*.olb"]
	puts $allLibs
	foreach f $allLibs {
		set fbasename [file rootname $f]
		puts $f
		XMATIC_OLB2XML $f ${fbasename}.xml
	}
}

proc ::capMenuUtil::convert { pLib } {
	set olbLibPath "M:/Developpement/08_Gest_Comp_Lib"
	set orlib2ki "E:/orlib2ki/orlib2ki.exe"
	set allLibs [glob -directory $olbLibPath -- "*.xml"]
	foreach f $allLibs {
		puts $f
		set cmd [list ${orlib2ki} -i $f -g 10.0 -s 40]
		puts $cmd
		eval exec $cmd
	}
}

proc ::capMenuUtil::capTrue { } {
	return 1
}

RegisterAction "_cdnCapTclAddDesignCustomMenu" "::capMenuUtil::capTrue" "" "::capMenuUtil::addLibXMLMenu" ""
