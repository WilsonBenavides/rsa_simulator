//
// Generated file, do not edit! Created by nedtool 5.6 from node/OpticalMsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include "OpticalMsg_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace {
template <class T> inline
typename std::enable_if<std::is_polymorphic<T>::value && std::is_base_of<omnetpp::cObject,T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)(static_cast<const omnetpp::cObject *>(t));
}

template <class T> inline
typename std::enable_if<std::is_polymorphic<T>::value && !std::is_base_of<omnetpp::cObject,T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)dynamic_cast<const void *>(t);
}

template <class T> inline
typename std::enable_if<!std::is_polymorphic<T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)static_cast<const void *>(t);
}

}


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule to generate operator<< for shared_ptr<T>
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const std::shared_ptr<T>& t) { return out << t.get(); }

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');

    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(OpticalMsg)

OpticalMsg::OpticalMsg(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

OpticalMsg::OpticalMsg(const OpticalMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

OpticalMsg::~OpticalMsg()
{
}

OpticalMsg& OpticalMsg::operator=(const OpticalMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void OpticalMsg::copy(const OpticalMsg& other)
{
    this->srcAddr = other.srcAddr;
    this->destAddr = other.destAddr;
    this->slotReq = other.slotReq;
    this->msgState = other.msgState;
    this->color = other.color;
}

void OpticalMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->srcAddr);
    doParsimPacking(b,this->destAddr);
    doParsimPacking(b,this->slotReq);
    doParsimPacking(b,this->msgState);
    doParsimPacking(b,this->color);
}

void OpticalMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->srcAddr);
    doParsimUnpacking(b,this->destAddr);
    doParsimUnpacking(b,this->slotReq);
    doParsimUnpacking(b,this->msgState);
    doParsimUnpacking(b,this->color);
}

int OpticalMsg::getSrcAddr() const
{
    return this->srcAddr;
}

void OpticalMsg::setSrcAddr(int srcAddr)
{
    this->srcAddr = srcAddr;
}

int OpticalMsg::getDestAddr() const
{
    return this->destAddr;
}

void OpticalMsg::setDestAddr(int destAddr)
{
    this->destAddr = destAddr;
}

int OpticalMsg::getSlotReq() const
{
    return this->slotReq;
}

void OpticalMsg::setSlotReq(int slotReq)
{
    this->slotReq = slotReq;
}

int OpticalMsg::getMsgState() const
{
    return this->msgState;
}

void OpticalMsg::setMsgState(int msgState)
{
    this->msgState = msgState;
}

long OpticalMsg::getColor() const
{
    return this->color;
}

void OpticalMsg::setColor(long color)
{
    this->color = color;
}

class OpticalMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_srcAddr,
        FIELD_destAddr,
        FIELD_slotReq,
        FIELD_msgState,
        FIELD_color,
    };
  public:
    OpticalMsgDescriptor();
    virtual ~OpticalMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(OpticalMsgDescriptor)

OpticalMsgDescriptor::OpticalMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(OpticalMsg)), "omnetpp::cPacket")
{
    propertynames = nullptr;
}

OpticalMsgDescriptor::~OpticalMsgDescriptor()
{
    delete[] propertynames;
}

bool OpticalMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<OpticalMsg *>(obj)!=nullptr;
}

const char **OpticalMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *OpticalMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int OpticalMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount() : 5;
}

unsigned int OpticalMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcAddr
        FD_ISEDITABLE,    // FIELD_destAddr
        FD_ISEDITABLE,    // FIELD_slotReq
        FD_ISEDITABLE,    // FIELD_msgState
        FD_ISEDITABLE,    // FIELD_color
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *OpticalMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcAddr",
        "destAddr",
        "slotReq",
        "msgState",
        "color",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int OpticalMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 's' && strcmp(fieldName, "srcAddr") == 0) return base+0;
    if (fieldName[0] == 'd' && strcmp(fieldName, "destAddr") == 0) return base+1;
    if (fieldName[0] == 's' && strcmp(fieldName, "slotReq") == 0) return base+2;
    if (fieldName[0] == 'm' && strcmp(fieldName, "msgState") == 0) return base+3;
    if (fieldName[0] == 'c' && strcmp(fieldName, "color") == 0) return base+4;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *OpticalMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_srcAddr
        "int",    // FIELD_destAddr
        "int",    // FIELD_slotReq
        "int",    // FIELD_msgState
        "long",    // FIELD_color
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **OpticalMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *OpticalMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int OpticalMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    OpticalMsg *pp = (OpticalMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *OpticalMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    OpticalMsg *pp = (OpticalMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string OpticalMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    OpticalMsg *pp = (OpticalMsg *)object; (void)pp;
    switch (field) {
        case FIELD_srcAddr: return long2string(pp->getSrcAddr());
        case FIELD_destAddr: return long2string(pp->getDestAddr());
        case FIELD_slotReq: return long2string(pp->getSlotReq());
        case FIELD_msgState: return long2string(pp->getMsgState());
        case FIELD_color: return long2string(pp->getColor());
        default: return "";
    }
}

bool OpticalMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    OpticalMsg *pp = (OpticalMsg *)object; (void)pp;
    switch (field) {
        case FIELD_srcAddr: pp->setSrcAddr(string2long(value)); return true;
        case FIELD_destAddr: pp->setDestAddr(string2long(value)); return true;
        case FIELD_slotReq: pp->setSlotReq(string2long(value)); return true;
        case FIELD_msgState: pp->setMsgState(string2long(value)); return true;
        case FIELD_color: pp->setColor(string2long(value)); return true;
        default: return false;
    }
}

const char *OpticalMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *OpticalMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    OpticalMsg *pp = (OpticalMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(OpticalMsgPath)

OpticalMsgPath::OpticalMsgPath(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

OpticalMsgPath::OpticalMsgPath(const OpticalMsgPath& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

OpticalMsgPath::~OpticalMsgPath()
{
    delete [] this->opticalPath;
}

OpticalMsgPath& OpticalMsgPath::operator=(const OpticalMsgPath& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void OpticalMsgPath::copy(const OpticalMsgPath& other)
{
    this->srcAddr = other.srcAddr;
    this->destAddr = other.destAddr;
    this->slotReq = other.slotReq;
    this->msgState = other.msgState;
    delete [] this->opticalPath;
    this->opticalPath = (other.opticalPath_arraysize==0) ? nullptr : new int[other.opticalPath_arraysize];
    opticalPath_arraysize = other.opticalPath_arraysize;
    for (size_t i = 0; i < opticalPath_arraysize; i++) {
        this->opticalPath[i] = other.opticalPath[i];
    }
    this->color = other.color;
}

void OpticalMsgPath::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->srcAddr);
    doParsimPacking(b,this->destAddr);
    doParsimPacking(b,this->slotReq);
    doParsimPacking(b,this->msgState);
    b->pack(opticalPath_arraysize);
    doParsimArrayPacking(b,this->opticalPath,opticalPath_arraysize);
    doParsimPacking(b,this->color);
}

void OpticalMsgPath::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->srcAddr);
    doParsimUnpacking(b,this->destAddr);
    doParsimUnpacking(b,this->slotReq);
    doParsimUnpacking(b,this->msgState);
    delete [] this->opticalPath;
    b->unpack(opticalPath_arraysize);
    if (opticalPath_arraysize == 0) {
        this->opticalPath = nullptr;
    } else {
        this->opticalPath = new int[opticalPath_arraysize];
        doParsimArrayUnpacking(b,this->opticalPath,opticalPath_arraysize);
    }
    doParsimUnpacking(b,this->color);
}

int OpticalMsgPath::getSrcAddr() const
{
    return this->srcAddr;
}

void OpticalMsgPath::setSrcAddr(int srcAddr)
{
    this->srcAddr = srcAddr;
}

int OpticalMsgPath::getDestAddr() const
{
    return this->destAddr;
}

void OpticalMsgPath::setDestAddr(int destAddr)
{
    this->destAddr = destAddr;
}

int OpticalMsgPath::getSlotReq() const
{
    return this->slotReq;
}

void OpticalMsgPath::setSlotReq(int slotReq)
{
    this->slotReq = slotReq;
}

int OpticalMsgPath::getMsgState() const
{
    return this->msgState;
}

void OpticalMsgPath::setMsgState(int msgState)
{
    this->msgState = msgState;
}

size_t OpticalMsgPath::getOpticalPathArraySize() const
{
    return opticalPath_arraysize;
}

int OpticalMsgPath::getOpticalPath(size_t k) const
{
    if (k >= opticalPath_arraysize) throw omnetpp::cRuntimeError("Array of size opticalPath_arraysize indexed by %lu", (unsigned long)k);
    return this->opticalPath[k];
}

void OpticalMsgPath::setOpticalPathArraySize(size_t newSize)
{
    int *opticalPath2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = opticalPath_arraysize < newSize ? opticalPath_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        opticalPath2[i] = this->opticalPath[i];
    for (size_t i = minSize; i < newSize; i++)
        opticalPath2[i] = 0;
    delete [] this->opticalPath;
    this->opticalPath = opticalPath2;
    opticalPath_arraysize = newSize;
}

void OpticalMsgPath::setOpticalPath(size_t k, int opticalPath)
{
    if (k >= opticalPath_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    this->opticalPath[k] = opticalPath;
}

void OpticalMsgPath::insertOpticalPath(size_t k, int opticalPath)
{
    if (k > opticalPath_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = opticalPath_arraysize + 1;
    int *opticalPath2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        opticalPath2[i] = this->opticalPath[i];
    opticalPath2[k] = opticalPath;
    for (i = k + 1; i < newSize; i++)
        opticalPath2[i] = this->opticalPath[i-1];
    delete [] this->opticalPath;
    this->opticalPath = opticalPath2;
    opticalPath_arraysize = newSize;
}

void OpticalMsgPath::insertOpticalPath(int opticalPath)
{
    insertOpticalPath(opticalPath_arraysize, opticalPath);
}

void OpticalMsgPath::eraseOpticalPath(size_t k)
{
    if (k >= opticalPath_arraysize) throw omnetpp::cRuntimeError("Array of size  indexed by %lu", (unsigned long)k);
    size_t newSize = opticalPath_arraysize - 1;
    int *opticalPath2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        opticalPath2[i] = this->opticalPath[i];
    for (i = k; i < newSize; i++)
        opticalPath2[i] = this->opticalPath[i+1];
    delete [] this->opticalPath;
    this->opticalPath = opticalPath2;
    opticalPath_arraysize = newSize;
}

long OpticalMsgPath::getColor() const
{
    return this->color;
}

void OpticalMsgPath::setColor(long color)
{
    this->color = color;
}

class OpticalMsgPathDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_srcAddr,
        FIELD_destAddr,
        FIELD_slotReq,
        FIELD_msgState,
        FIELD_opticalPath,
        FIELD_color,
    };
  public:
    OpticalMsgPathDescriptor();
    virtual ~OpticalMsgPathDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(OpticalMsgPathDescriptor)

OpticalMsgPathDescriptor::OpticalMsgPathDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(OpticalMsgPath)), "omnetpp::cPacket")
{
    propertynames = nullptr;
}

OpticalMsgPathDescriptor::~OpticalMsgPathDescriptor()
{
    delete[] propertynames;
}

bool OpticalMsgPathDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<OpticalMsgPath *>(obj)!=nullptr;
}

const char **OpticalMsgPathDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *OpticalMsgPathDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int OpticalMsgPathDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount() : 6;
}

unsigned int OpticalMsgPathDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcAddr
        FD_ISEDITABLE,    // FIELD_destAddr
        FD_ISEDITABLE,    // FIELD_slotReq
        FD_ISEDITABLE,    // FIELD_msgState
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_opticalPath
        FD_ISEDITABLE,    // FIELD_color
    };
    return (field >= 0 && field < 6) ? fieldTypeFlags[field] : 0;
}

const char *OpticalMsgPathDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcAddr",
        "destAddr",
        "slotReq",
        "msgState",
        "opticalPath",
        "color",
    };
    return (field >= 0 && field < 6) ? fieldNames[field] : nullptr;
}

int OpticalMsgPathDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 's' && strcmp(fieldName, "srcAddr") == 0) return base+0;
    if (fieldName[0] == 'd' && strcmp(fieldName, "destAddr") == 0) return base+1;
    if (fieldName[0] == 's' && strcmp(fieldName, "slotReq") == 0) return base+2;
    if (fieldName[0] == 'm' && strcmp(fieldName, "msgState") == 0) return base+3;
    if (fieldName[0] == 'o' && strcmp(fieldName, "opticalPath") == 0) return base+4;
    if (fieldName[0] == 'c' && strcmp(fieldName, "color") == 0) return base+5;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *OpticalMsgPathDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_srcAddr
        "int",    // FIELD_destAddr
        "int",    // FIELD_slotReq
        "int",    // FIELD_msgState
        "int",    // FIELD_opticalPath
        "long",    // FIELD_color
    };
    return (field >= 0 && field < 6) ? fieldTypeStrings[field] : nullptr;
}

const char **OpticalMsgPathDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *OpticalMsgPathDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int OpticalMsgPathDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    OpticalMsgPath *pp = (OpticalMsgPath *)object; (void)pp;
    switch (field) {
        case FIELD_opticalPath: return pp->getOpticalPathArraySize();
        default: return 0;
    }
}

const char *OpticalMsgPathDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    OpticalMsgPath *pp = (OpticalMsgPath *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string OpticalMsgPathDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    OpticalMsgPath *pp = (OpticalMsgPath *)object; (void)pp;
    switch (field) {
        case FIELD_srcAddr: return long2string(pp->getSrcAddr());
        case FIELD_destAddr: return long2string(pp->getDestAddr());
        case FIELD_slotReq: return long2string(pp->getSlotReq());
        case FIELD_msgState: return long2string(pp->getMsgState());
        case FIELD_opticalPath: return long2string(pp->getOpticalPath(i));
        case FIELD_color: return long2string(pp->getColor());
        default: return "";
    }
}

bool OpticalMsgPathDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    OpticalMsgPath *pp = (OpticalMsgPath *)object; (void)pp;
    switch (field) {
        case FIELD_srcAddr: pp->setSrcAddr(string2long(value)); return true;
        case FIELD_destAddr: pp->setDestAddr(string2long(value)); return true;
        case FIELD_slotReq: pp->setSlotReq(string2long(value)); return true;
        case FIELD_msgState: pp->setMsgState(string2long(value)); return true;
        case FIELD_opticalPath: pp->setOpticalPath(i,string2long(value)); return true;
        case FIELD_color: pp->setColor(string2long(value)); return true;
        default: return false;
    }
}

const char *OpticalMsgPathDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *OpticalMsgPathDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    OpticalMsgPath *pp = (OpticalMsgPath *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

