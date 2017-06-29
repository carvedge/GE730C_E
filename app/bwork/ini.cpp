#include "ini.h"

Ini * Ini::_instance = NULL;
Ini* Ini::Instance(){
    if (_instance == NULL){
        _instance = new Ini();
    }
    return _instance;
}
Ini::Ini(){
}
void Ini::setFileName(string fileName){
   filename = fileName;
    if (access(filename.c_str(), 0) == 0) {
        this->err_code = 0;
        boost::property_tree::ini_parser::read_ini(filename, this->m_pt);
    } else {
        this->err_code = 1;
    }
}

short Ini::errCode(){
    return this->err_code;
}

string Ini::get(string path){
    if (access(filename.c_str(), 0) == 0) {
        this->err_code = 0;
        boost::property_tree::ini_parser::read_ini(filename, this->m_pt);
    } else {
        this->err_code = 1;
    }
    
    if (this->err_code == 0) {
        return this->m_pt.get<string>(path);
    } else {
        return "";
    }
}
void Ini::save(string key,string value)
{
   using boost::property_tree::ptree;

    if (access(filename.c_str(), 0) == 0) {
        this->err_code = 0;
        boost::property_tree::ini_parser::read_ini(filename, this->m_pt);
    } else {
        this->err_code = 1;
    }


   ptree pt;
   pt.put("cfg.n00", get("cfg.n00"));
   pt.put("cfg.n01", get("cfg.n01"));
   pt.put("cfg.n02", get("cfg.n02"));
   pt.put("cfg.n03", get("cfg.n03"));
   pt.put("cfg.n04", get("cfg.n04"));
   pt.put("cfg.n05", get("cfg.n05"));
   pt.put("cfg.n06", get("cfg.n06"));
   pt.put("cfg.n07", get("cfg.n07"));
   pt.put("cfg.n08", get("cfg.n08"));
   pt.put("cfg.n09", get("cfg.n19"));
   pt.put("cfg.n10", get("cfg.n10"));
   pt.put("cfg.n11", get("cfg.n11"));
   pt.put("cfg.n12", get("cfg.n12"));
   pt.put("cfg.n13", get("cfg.n13"));
   pt.put("cfg.n14", get("cfg.n14"));
   pt.put("cfg.n15", get("cfg.n15"));
   pt.put("cfg.n16", get("cfg.n16"));
   pt.put("cfg.n17", get("cfg.n17"));
   pt.put("cfg.n18", get("cfg.n18"));
   pt.put("cfg.n19", get("cfg.n19"));
   pt.put(key, value);
   boost::property_tree::ini_parser::write_ini( filename, pt );
}
