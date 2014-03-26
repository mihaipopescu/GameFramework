solution "GameFramework"
	configurations { "Debug", "Release" }

		location ".."
	
		project "Game"
		kind "WindowedApp"
		language "C++"
		
		
		objdir "../Compiled"
		debugdir ".."
		
		files {
			"../Includes/*.h",
			"../Source/*.cpp",
			"../Source/*.inl",
			"../Docs/*.txt",
		}
		
		includedirs {
			"../Includes",
		}
		
		links {
			"winmm.lib",
			"Msimg32.lib",
		}
		
		flags {
			"WinMain",
			"FatalWarnings"
		}

		configuration { "windows", "vs*" }
			defines { "WIN32" }
			flags { "NoIncrementalLink", "NoEditAndContinue", "NoPCH" }

			configuration "Debug"
				defines { "_DEBUG" }
				flags { "Symbols" }
				targetprefix "d_"

			configuration "Release"
				defines { "NDEBUG" }
				optimize "Speed"
			

