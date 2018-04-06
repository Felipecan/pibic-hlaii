// interface definitions for VIP

#ifndef _SQI_ITEM_H_
#define _SQI_ITEM_H_

#include <systemc>

class ii_sequence_item
{
 public:
 
  //+-----------------------------------------
  //| Data Members
  //+-----------------------------------------
  bool   data_enable;
  double data_in[DATA_SIZE];

  //Output
  double data_out[DATA_SIZE];
  bool   data_valid;

  //+-----------------------------------------
  //| Data No Members
  //+-----------------------------------------
  
  ii_sequence_item(const std::string& name = "ii_sequence_item"){ }

};
std::ostream & operator << (std::ostream &os, const ii_sequence_item &item){
  return os << "instance item of ii block";
}
#endif /* _SQI_ITEM_H_ */
