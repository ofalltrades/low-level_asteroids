#ifndef COMMONLIB
#define COMMONLIB

// With ( ( array )[0] ), the reason we use the parens is that,
//   since it is a macro, something like foo + bar being passed in
//   would result in ( foo + bar[0] ); we would want ( ( foo + bar )[0] )
#define arrayLength( array ) ( sizeof( array ) / sizeof( ( array )[0] ) )

#endif
