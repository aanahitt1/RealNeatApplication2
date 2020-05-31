#ifndef RNAFOLD_GLOBAL_H
#define RNAFOLD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RNAFOLD_LIBRARY)
#  define RNAFOLD_EXPORT Q_DECL_EXPORT
#else
#  define RNAFOLD_EXPORT Q_DECL_IMPORT
#endif

#endif // RNAFOLD_GLOBAL_H
