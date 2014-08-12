//
// Generated file, do not edit! Created by opp_msgc 4.4 from EMSPkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "EMSPkt_m.h"

USING_NAMESPACE

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(EMSPkt);

EMSPkt::EMSPkt(const char *name, int kind) : ::MacPkt(name,kind)
{
    this->address3_var = 0;
    this->address4_var = 0;
    this->fragmentation_var = 0;
    this->informationDS_var = 0;
    this->sequenceControl_var = 0;
    this->retry_var = 0;
    this->duration_var = 0;
}

EMSPkt::EMSPkt(const EMSPkt& other) : ::MacPkt(other)
{
    copy(other);
}

EMSPkt::~EMSPkt()
{
}

EMSPkt& EMSPkt::operator=(const EMSPkt& other)
{
    if (this==&other) return *this;
    ::MacPkt::operator=(other);
    copy(other);
    return *this;
}

void EMSPkt::copy(const EMSPkt& other)
{
    this->address3_var = other.address3_var;
    this->address4_var = other.address4_var;
    this->fragmentation_var = other.fragmentation_var;
    this->informationDS_var = other.informationDS_var;
    this->sequenceControl_var = other.sequenceControl_var;
    this->retry_var = other.retry_var;
    this->duration_var = other.duration_var;
}

void EMSPkt::parsimPack(cCommBuffer *b)
{
    ::MacPkt::parsimPack(b);
    doPacking(b,this->address3_var);
    doPacking(b,this->address4_var);
    doPacking(b,this->fragmentation_var);
    doPacking(b,this->informationDS_var);
    doPacking(b,this->sequenceControl_var);
    doPacking(b,this->retry_var);
    doPacking(b,this->duration_var);
}

void EMSPkt::parsimUnpack(cCommBuffer *b)
{
    ::MacPkt::parsimUnpack(b);
    doUnpacking(b,this->address3_var);
    doUnpacking(b,this->address4_var);
    doUnpacking(b,this->fragmentation_var);
    doUnpacking(b,this->informationDS_var);
    doUnpacking(b,this->sequenceControl_var);
    doUnpacking(b,this->retry_var);
    doUnpacking(b,this->duration_var);
}

int EMSPkt::getAddress3() const
{
    return address3_var;
}

void EMSPkt::setAddress3(int address3)
{
    this->address3_var = address3;
}

int EMSPkt::getAddress4() const
{
    return address4_var;
}

void EMSPkt::setAddress4(int address4)
{
    this->address4_var = address4;
}

int EMSPkt::getFragmentation() const
{
    return fragmentation_var;
}

void EMSPkt::setFragmentation(int fragmentation)
{
    this->fragmentation_var = fragmentation;
}

int EMSPkt::getInformationDS() const
{
    return informationDS_var;
}

void EMSPkt::setInformationDS(int informationDS)
{
    this->informationDS_var = informationDS;
}

int EMSPkt::getSequenceControl() const
{
    return sequenceControl_var;
}

void EMSPkt::setSequenceControl(int sequenceControl)
{
    this->sequenceControl_var = sequenceControl;
}

bool EMSPkt::getRetry() const
{
    return retry_var;
}

void EMSPkt::setRetry(bool retry)
{
    this->retry_var = retry;
}

simtime_t EMSPkt::getDuration() const
{
    return duration_var;
}

void EMSPkt::setDuration(simtime_t duration)
{
    this->duration_var = duration;
}

class EMSPktDescriptor : public cClassDescriptor
{
  public:
    EMSPktDescriptor();
    virtual ~EMSPktDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(EMSPktDescriptor);

EMSPktDescriptor::EMSPktDescriptor() : cClassDescriptor("EMSPkt", "MacPkt")
{
}

EMSPktDescriptor::~EMSPktDescriptor()
{
}

bool EMSPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<EMSPkt *>(obj)!=NULL;
}

const char *EMSPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int EMSPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 7+basedesc->getFieldCount(object) : 7;
}

unsigned int EMSPktDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<7) ? fieldTypeFlags[field] : 0;
}

const char *EMSPktDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "address3",
        "address4",
        "fragmentation",
        "informationDS",
        "sequenceControl",
        "retry",
        "duration",
    };
    return (field>=0 && field<7) ? fieldNames[field] : NULL;
}

int EMSPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='a' && strcmp(fieldName, "address3")==0) return base+0;
    if (fieldName[0]=='a' && strcmp(fieldName, "address4")==0) return base+1;
    if (fieldName[0]=='f' && strcmp(fieldName, "fragmentation")==0) return base+2;
    if (fieldName[0]=='i' && strcmp(fieldName, "informationDS")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "sequenceControl")==0) return base+4;
    if (fieldName[0]=='r' && strcmp(fieldName, "retry")==0) return base+5;
    if (fieldName[0]=='d' && strcmp(fieldName, "duration")==0) return base+6;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *EMSPktDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "int",
        "bool",
        "simtime_t",
    };
    return (field>=0 && field<7) ? fieldTypeStrings[field] : NULL;
}

const char *EMSPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int EMSPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    EMSPkt *pp = (EMSPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string EMSPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    EMSPkt *pp = (EMSPkt *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getAddress3());
        case 1: return long2string(pp->getAddress4());
        case 2: return long2string(pp->getFragmentation());
        case 3: return long2string(pp->getInformationDS());
        case 4: return long2string(pp->getSequenceControl());
        case 5: return bool2string(pp->getRetry());
        case 6: return double2string(pp->getDuration());
        default: return "";
    }
}

bool EMSPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    EMSPkt *pp = (EMSPkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setAddress3(string2long(value)); return true;
        case 1: pp->setAddress4(string2long(value)); return true;
        case 2: pp->setFragmentation(string2long(value)); return true;
        case 3: pp->setInformationDS(string2long(value)); return true;
        case 4: pp->setSequenceControl(string2long(value)); return true;
        case 5: pp->setRetry(string2bool(value)); return true;
        case 6: pp->setDuration(string2double(value)); return true;
        default: return false;
    }
}

const char *EMSPktDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<7) ? fieldStructNames[field] : NULL;
}

void *EMSPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    EMSPkt *pp = (EMSPkt *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


