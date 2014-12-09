#if __has_feature(objc_arc_weak)
	#define WEAK	weak
	#define __WEAK	__weak
#elif __has_feature(objc_arc)
	#define WEAK	unsafe_unretained
	#define __WEAK	__unsafe_unretained
#else
	#define WEAK assign
	#define __WEAK
#endif