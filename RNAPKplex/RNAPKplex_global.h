#ifndef RNAPKPLEX_GLOBAL_H
#define RNAPKPLEX_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RNAPKPLEX_LIBRARY)
#  define RNAPKPLEX_EXPORT Q_DECL_EXPORT
#else
#  define RNAPKPLEX_EXPORT Q_DECL_IMPORT
#endif

#endif // RNAPKPLEX_GLOBAL_H
