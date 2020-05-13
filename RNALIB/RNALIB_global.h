#ifndef RNALIB_GLOBAL_H
#define RNALIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RNALIB_LIBRARY)
#  define RNALIB_EXPORT Q_DECL_EXPORT
#else
#  define RNALIB_EXPORT Q_DECL_IMPORT
#endif

#endif // RNALIB_GLOBAL_H
