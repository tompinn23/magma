#pragma once

#if defined(MAGMA_DLL_BUILD) && !defined(MAGMA_STATIC_BUILD)
#define MAGMA_DECL __declspec(dllexport)
#elif !defined(MAGMA_DLL_BUILD) && !defined(MAGMA_STATIC_BUILD)
#define MAGMA_DECL __declspec(dllimport)
#else
#define MAGMA_DECL
#endif
