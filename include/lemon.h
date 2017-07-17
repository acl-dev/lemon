#pragma once
#include <set>
#include <string>
#include <vector>
#include <fstream>
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
            e_le,              //  <= less than or equal to
            e_html_comment_begin,//  <!--
            e_sub,             //  -
            e_html_comment_end,//  -->
            e_gt,              //  >
            e_ge,              //  >=
            e_eq,              //  ==
            e_neq,             //  !=

            e_open_brace,      //  {
            e_open_block,      //  {%
            e_open_variable,   //  {{
            e_close_brace,     //  }
            e_modulus,         //  %
            e_close_block,     //  %}}
            e_close_variable,  //  }}
            e_forward_slash,   //  /
            e_cpp_comment,     //  //
            e_cpp_comment_begin,// /*
            e_cpp_comment_end, //  */
            e_pipeline,        //  |
            e_ampersand,       //  &
            e_on,              //  on
            e_off,             //  off
            e_if,              //  if
            e_else,            //  else,
            e_elif,            //  else if
            e_endif,           //  endif
            e_for,             //  for
            e_in,              //  in
            e_empty,           //  empty
            e_endfor,          //  endfor
            e_and,             //  end 
            e_not,             //  not
            e_or,              //  or
            e_include,         //  include
            e_block,           //  block
            e_end_block,       //  end_block
            e_extends,         //  extends
            e_autoescape,      //  autoescape
            e_endautoescape,   //  endautoescape

            //filters
            e_length,          //  length filter
            e_safe,
            e_default,

            e_dot,             // .
            e_asterisk,        // *
            e_colon,           // :
            e_double_colon,    // ::

            e_comma,           //  ,
            e_semicolon,       //  ;
            e_backtick,        //  `
            e_double_quote,    //  "
            e_quote,           //  '
            e_open_paren,      //  (
            e_close_paren,     //  )
            e_assign,          //  =
            e_identifier,
            e_not_op,          // !

            //c++ things
            e_struct,
            e_class,
            e_public,
            e_protected,
            e_private,
            e_virtual,
            e_override,
            e_inline,
            e_operator,
            e_namespace,

            //
            e_bool,
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
            e_float,
            e_double,

            e_void,            //  void
            e_const,           //  const
            e_std_string,      //  std::list
            e_acl_string,      //  acl::string
            e_std_vector,      //  std::vector
            e_std_list,        //  std::list
            e_std_map,         //  std::map
            e_std_set,         //  std::set


        } type_t;

        type_t type_;
        std::string str_;
        int line_;
    };
    struct field
    {

        field()
            :line_(0)
        {

        }
        typedef enum type
        {
            e_void,
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

            e_std_list,
            e_std_vector,
            e_std_map,
            e_std_set,
            e_class
        }type_t;

        type_t type_;

        int line_;
        std::string name_;
        std::string str_;
        std::string type_str_;
        std::vector<std::string> namespaces_;
    };

    struct class_t
    {
        std::string file_path_;
        std::string name_;
        std::vector<std::string> namespaces_;
        std::vector<field> variables_;
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
    struct lexer
    {
        token_t token_;
        std::ifstream *file_;
        std::string file_path_;
        std::string current_line_;
        std::string line_buffer_;
        int line_;
    };
    struct block
    {
        std::string name_;
        long        offset_;
        std::string file_path_;
        std::string current_line_;
        std::string line_buffer_;
        int line_;
        std::ifstream *file_;
    };
    struct stack
    {
        std::string name_;
        std::string type_;
    };

    std::string next_token(const std::string &delimiters);
    std::string get_string(size_t len);
    std::string get_string(const std::string &delimiters);

    void move_buffer(int offset);

    void read_line();
public:
    lemon();
    ~lemon();
    bool parse_cpp_header(const std::string &file_path);
    bool parse_template(const std::string &file_path);

private:
    lexer *new_lexer(const std::string &file_path);
    int line();
    void print_lexer_status();
    void assert_not_eof(const token_t &t);
    token_t get_next_token(const std::string &skipstr=" \r\n\t");
    void clear_line_buffer();
    token_t curr_token();
    void push_status(token_t::type_t type);
    void push_auto_escape(bool escape);
    void pop_auto_escape();
    bool auto_escape();
    token_t::type_t pop_status();
    token_t::type_t get_status();
    std::string get_status_str();
    std::string parse_open_block();
    void pop_stack();
    void push_stack(const std::string &name, const std::string &type);
    void push_stack_size(int size);
    void push_back(const token_t &value);
    field::type get_field_type(const std::string &type);
    field::type get_field_type(const token_t &token);
    void init_filter();
    bool check_filter(const std::string &name);
    std::string gen_bool_code(const std::string &item);
    std::string get_type(const std::string &name);
    std::string get_code();
    std::string parse_if();
    std::string get_for_items();
    std::string parse_for();
    std::string get_variable();
    std::string to_string(const std::string &name,const std::string &type);
    std::string parse_variable();
    std::string get_include_filepath();
    std::string parse_html_include();
    std::string get_default_string();
    std::string parse_block();
    std::string parse_extends();
    std::string parse_html();
    block get_block(const std::string &name);
    bool block_exist(const std::string &name);
    void parse_template();

    std::string get_iterator();

    void pop_for_item();
    void add_for_item(const std::string &item);
    std::string get_for_item();

    ///c++
    typedef std::vector<field> fields_t;
    typedef std::vector<std::string> namespaces_t;
    std::string to_string(const namespaces_t &namespaces);
    field parse_return();
    field parse_param();
    void parse_interface();
    void parse_class(bool is_struct);
    void parse_cpp_header();
    void parse_cpp_include();
    bool check_file_done(const std::string &file_name);
    void skip_cpp_comment();
    token_t get_next_token(bool auto_skip_comment);
    fields_t get_variable(const std::string &name, const namespaces_t&nps);
    bool check_class_exist(const std::string &name, const namespaces_t&nps);
    fields_t get_parent_variables(bool is_struct);
    bool skip_to_public();
    namespaces_t get_namespaces();
    field parse_field_type();
    void skip_function();
    class_t *get_class(const std::string &name, const namespaces_t &nsp);
private:
    std::vector<block>  blocks_;
    std::vector<lexer*> lexers_;
    lexer *lexer_;
    std::vector<bool> auto_escape_;
    std::vector<field> stack_;
    std::vector<int> stack_size_;

    std::list<token_t> tokens_;
    std::vector<class_t> classes_;
    std::vector<token_t::type_t> status_;

    template_t template_;
    int iterators_;
    bool is_base_;

    std::set<std::string> filters_;
    std::vector<std::string> for_items_;
    ///c++
    std::vector<std::string> analyzed_files_;
    std::vector<namespaces_t> namespaces_;
};