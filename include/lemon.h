#pragma once
#include <string>
#include <vector>

class lemon
{
private:
    struct token_t
    {
        typedef enum type_t
        {
            e_space,           //   
            e_$t,              // \t
            e_$r,              // \r
            e_$n,              // \n
            e_eof,             // end of file
            e_less,            //  <
            e_comment_begin,   //  <!--
            e_sub,             //  -
            e_comment_end,     //  -->
            e_gt,              //  >
            e_open_brace,      //  {
            e_code_begin,      //  {%
            e_double_open_brace,  //  {{
            e_close_brace,     //  }
            e_code_end,        //  %}}
            e_double_close_quote, //  }}
            e_forward_slash,   //  /
            e_and,             //  &
            e_if,              //  if
            e_else,            //  else,
            e_elif,            //  else if
            e_endif,           //  endif
            e_for,             //  for
            e_in,              //  in
            e_endfor,          //  endfor


            e_const,           //  const
            e_std_string,      //  std::list
            e_acl_string,      //  acl::string
            e_std_vector,      //  std::vector
            e_std_list,        //  std::list
            e_std_map,         //  std::map 
            e_std_set,         //  std::set

            e_char,
            e_unsigned_char,
            e_short,
            e_unsigned_shot,
            e_int,
            e_unsigned_int,
            e_long,
            e_unsigned_long,
            e_long_long,
            e_unsigned_long_long,

            e_colon,           // :
            e_double_colon,    // ::

            e_comma,           //  ,
            e_semicolon,       //  ;
            e_backtick,        //  `
            e_double_quote,    //  "
            e_quote,           //  '
            e_open_paren,      //  (
            e_close_paren,     //  )
            e_eq,              //  =
            e_identifier,
        } type_t;

        type_t type_;
        std::string str_;
        int line_;
    };
    
    struct entry;
    struct field;

    struct container_t
    {
        container_t()
                :type_(e_void),
                 field_(NULL)
        {

        }
        container_t(const container_t &other);
        ~container_t();
        /// todo


        typedef enum type
        {
            e_void,
            e_vector,
            e_list,
            e_map,
            e_set,
            e_field,
            e_obj,

        } type_t;
        type_t type_;
        field *field_;
        std::string str_;
    };

    struct field
    {
        field()
            :line_(0),
             container_(NULL)
        {

        }
        typedef enum type
        {
            e_char,
            e_unsigned_char,
            e_short,
            e_unsigned_shot,
            e_int,
            e_unsigned_int,
            e_long,
            e_unsigned_long,
            e_long_long,
            e_unsigned_long_long,

            e_std_string,
            e_acl_string,

            e_float,
            e_double,

            e_list,
            e_vector,
            e_map,
            e_set,
            e_entry
        }type_t;

        type_t type_;

        int line_;

        std::string name_;
        
        std::string str_;
        std::string type_str_;

        container_t container_;
    };

    struct entry
    {
        std::string name_;
        std::vector<std::string> namespaces_;
        std::vector<field> members_;
    };
    struct interface_t
    {
        std::string name_;
        std::string str_;
        field return_;
        std::vector<field> params_;
    };
    struct template_t
    {
        std::string name_;
        interface_t interface_;
    };
    std::string next_token(const std::string &delimiters);
    std::string get_string(int len);
    std::string get_string(const std::string &delimiters);

    void move_buffer(int offset);

    std::string read_line();
public:
    lemon();
    bool parse_template(const std::string &file_path);
    token_t get_next_token();
private:
    void parse_template();
    std::string get_container_str();
    field parse_container();
    field parse_return();
    field parse_param();
    void parse_interface();
private:
    token_t token_;
    acl::ifstream file_;
    std::string current_line_;
    std::string line_buffer_;
    int line_;

    std::list<token_t> tokens_;
    std::vector<entry> entrys_;

    template_t template_;
};