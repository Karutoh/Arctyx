#pragma once

#include <ehs/Serializer.h>

/// The symbol's type is not specified.
#define SYM_TYPE_NOTYPE 0x00

/// The symbol is associated with a data object, such as a variable, an array, and so on.
#define SYM_TYPE_OBJECT 0x01

/// The symbol is associated with a function or other executable code.
#define SYM_TYPE_FUNC 0x02

/// The symbol is associated with a section. Symbol table entries of this type exist primarily for relocation and normally have STB_LOCAL binding.
#define SYM_TYPE_SECTION 0x03

/// Conventionally, the symbol's name gives the name of the source file associated with the object file. A file symbol has STB_LOCAL binding, its section index is SHN_ABS, and it precedes the other STB_LOCAL symbols for the file, if it is present.
#define SYM_TYPE_FILE 0x04

/// The symbol labels an uninitialized common block.
#define SYM_TYPE_COMMON 0x05

/// The symbol specifies a Thread-Local Storage entity. When defined, it gives the assigned offset for the symbol, not the actual address. Symbols of type STT_TLS can be referenced by only special thread-local storage relocations and thread-local storage relocations can only reference symbols with type STT_TLS. Implementation need not support thread-local storage.
#define SYM_TYPE_TLS 0x06

/// Values in this inclusive range are reserved for operating system-specific semantics.
#define SYM_TYPE_LOOS 0x0A

/// Values in this inclusive range are reserved for operating system-specific semantics.
#define SYM_TYPE_HIOS 0x0C

/// Values in this inclusive range are reserved for processor-specific semantics. If meanings are specified, the processor supplement explains them.
#define SYM_TYPE_LOPROC 0x0D

/// Values in this inclusive range are reserved for processor-specific semantics. If meanings are specified, the processor supplement explains them.
#define SYM_TYPE_HIPROC 0x0F

/// Local symbols are not visible outside the object file containing their definition. Local symbols of the same name may exist in multiple files without interfering with each other.
#define SYM_BIND_LOCAL 0x00

/// Global symbols are visible to all object files being combined. One file's definition of a global symbol will satisfy another file's undefined reference to the same global symbol.
#define SYM_BIND_GLOBAL 0x01

/// Weak symbols resemble global symbols, but their definitions have lower precedence.
#define SYM_BIND_WEAK 0x02

/// Values in this inclusive range are reserved for operating system-specific semantics.
#define SYM_BIND_LOOS 0x0A

/// Values in this inclusive range are reserved for operating system-specific semantics.
#define SYM_BIND_HIOS 0x0C

/// Values in this inclusive range are reserved for processor-specific semantics. If meanings are specified, the processor supplement explains them.
#define SYM_BIND_LOPROC 0x0D

/// Values in this inclusive range are reserved for processor-specific semantics. If meanings are specified, the processor supplement explains them.
#define SYM_BIND_HIPROC 0x0F

#define SYM_BIND(i) ((i)>>4)
#define SYM_TYPE(i) ((i)&0xf)
#define SYM_INFO(b,t) (((b)<<4)+((t)&0xf))

/// The visibility of symbols with the STV_DEFAULT attribute is as specified by the symbol's binding type. That is, global and weak symbols are visible outside of their defining component (executable file or shared object). Local symbols are hidden, as described below. Global and weak symbols are also preemptable, that is, they may by preempted by definitions of the same name in another component.
#define SYM_VIS_DEFAULT 0x00

/// The meaning of this visibility attribute may be defined by processor supplements to further constrain hidden symbols. A processor supplement's definition should be such that generic tools can safely treat internal symbols as hidden.
#define SYM_VIS_INTERNAL 0x01

/// A symbol defined in the current component is hidden if its name is not visible to other components. Such a symbol is necessarily protected. This attribute may be used to control the external interface of a component. Note that an object named by such a symbol may still be referenced from another component if its address is passed outside.
#define SYM_VIS_HIDDEN 0x02

/// A symbol defined in the current component is protected if it is visible in other components but not preemptable, meaning that any reference to such a symbol from within the defining component must be resolved to the definition in that component, even if there is a definition in another component that would preempt by the default rules. A symbol with STB_LOCAL binding may not have STV_PROTECTED visibility. If a symbol definition with STV_PROTECTED visibility from a shared object is taken as resolving a reference from an executable or another shared object, the SHN_UNDEF symbol table entry created has STV_DEFAULT visibility.
#define SYM_VIS_PROTECTED 0x03

#define SYMH_SIZE 24

class ELF64_Sym
{
private:
	ehs::UInt_32 nameOffset;
	ehs::UInt_8 type;
	ehs::UInt_8 visibility;
	ehs::UInt_16 section;
	ehs::UInt_64 value;
	ehs::UInt_64 size;

public:
	ELF64_Sym();

	ELF64_Sym(ehs::UInt_32 nameOffset, ehs::UInt_8 type, ehs::UInt_8 visibility, ehs::UInt_16 section, ehs::UInt_64 value, ehs::UInt_64 size);

	void Serialize(ehs::Serializer<ehs::UInt_64>& inData) const;
};