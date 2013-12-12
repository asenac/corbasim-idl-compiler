#include "generator.hpp"
#include "output_stream.hpp"
#include <idlmm.hpp>
#include <idlmm/IdlmmVisitor.hpp>
#include <iostream>
#include <vector>

using namespace corbasim::idl;
using namespace idlmm;

namespace
{

    // TODO use a cache
    std::string getFQN(Contained_ptr e, const char * separator = "::")
    {
        std::string res = separator + e->getIdentifier();
        if ((e = dynamic_cast< Contained_ptr >(e->getDefinedIn())))
            res = getFQN(e, separator) + res;
        return res;
    }

    template< typename T, typename C >
    std::vector< T * > get(C& c)
    {
        std::vector< T * > res;
        for (size_t i = 0; i < c.size(); ++i)
        {
            T * t = dynamic_cast< T * >(&c[i]);
            if (t)
                res.push_back(t);
        }
        return res;
    }

    template< typename T, typename Visitor >
    void visit_container(T * e, Visitor& v)
    {
        typename T::Contains_t& c = e->getContains();
        for (size_t i = 0; i < c.size(); ++i)
            c[i].accept(v);
    }

    struct stage1 : IdlmmVisitor
    {
        output_stream& o;
        stage1(output_stream& o_) : o(o_) {}

        void visit(TranslationUnit * e)
        {
            visit_container(e, *this);
        }

        void visit(ModuleDef * e)
        {
            visit_container(e, *this);
        }

        void visit(InterfaceDef * e)
        {
            {
                namespace_ nsc(o, "corbasim");
                namespace_ nsa(o, "adapted");

                const std::string fqn(getFQN(e));

                o % "template < >";
                o % "struct is_objrefvar< " << fqn 
                    << "_var > : public cs_mpl::true_";
                {
                    embrace b(o);
                    o % "typedef " << fqn << " interface;";
                }
                o << ';';
                *o;
            }
            visit_container(e, *this);
        }

        void visit(StructDef * e)
        {
            const std::string fqn(getFQN(e));
            o % "BOOST_FUSION_ADAPT_STRUCT( " << fqn << ",";
            ++o;
            for (size_t i = 0; i < e->getMembers().size(); ++i)
            {
                o % "("
                    << "TODO"
                    << ", "
                    << e->getMembers()[i].getIdentifier()
                    << ")";
            }
            --o;
            o % ')';
            *o;

            // TODO nested structs
        }
    };

    struct stage2 : IdlmmVisitor
    {
        output_stream& o;
        stage2(output_stream& o_) : o(o_) {}

        void visit(TranslationUnit * e)
        {
            visit_container(e, *this);
        }

        template< typename T >
        inline std::string get_identifier(T * e)
        {
            return std::string(e->getDefinedIn()? "": "_corbasim_") +
                e->getIdentifier();
        }

        template< typename T >
        void do_visit_container(T * e)
        {
            namespace_ ns(o, get_identifier(e));
            visit_container(e, *this);
        }

        void visit(ModuleDef * e)
        {
            do_visit_container(e);
        }

        void visit(InterfaceDef * e)
        {
            do_visit_container(e);
        }

        void visit(OperationDef * e)
        {
            const std::string& id = e->getIdentifier();
            OperationDef::Parameters_t& params = e->getParameters();
            o % "struct __" << id;
            {
                embrace b(o);

                // TODO argument list

                // Adapted members
                for (size_t i = 0; i < params.size(); ++i)
                {
                    o % "typedef corbasim::adapted::member< __"
                        << id << ", " << i << " > "
                        << params[i].getIdentifier()
                        << "_corbasim_member;";
                }
                if (params.size()) *o;

                // member fields
                for (size_t i = 0; i < params.size(); ++i)
                {
                    o % "TODO " << params[i].getIdentifier() << ";";
                }
                if (params.size()) *o;

                // Constructors
                o % "__" << id << "();";
                if (params.size())
                {
                    o % "__" << id << "(";
                    for (size_t i = 0; i < params.size(); ++i)
                    {
                        o << "TODO ";
                        o << params[i].getIdentifier();
                        if (i != params.size() - 1) o << ", ";
                    }
                    o << ");";
                }
                o % "__" << id << "(const __"
                    << id << "& o);";
            }
            o << ';';
            *o;
            o % "typedef __" << id << " "
                << id << ";";
            *o;
        }

        void visit(StructDef * e)
        {
            const std::string fqn(getFQN(e));
            o % "struct " << get_identifier(e);
            {
                embrace b(o);
                for (size_t i = 0; i < e->getMembers().size(); ++i)
                {
                    o % "typedef corbasim::adapted::member< "
                        << fqn << ", " << i << " > "
                        << e->getMembers()[i].getIdentifier()
                        << "_corbasim_member;";
                }

                // TODO nested structs
            }
            o << ';';
            *o;
        }
    };

    struct stage3 : IdlmmVisitor
    {
        output_stream& o;
        stage3(output_stream& o_) : o(o_) {}

        void visit(TranslationUnit * e)
        {
            visit_container(e, *this);
        }

        void visit(ModuleDef * e)
        {
            visit_container(e, *this);
        }

        void visit(InterfaceDef * e)
        {
            {
                namespace_ nsc(o, "corbasim");
                namespace_ nsa(o, "adapted");

                const std::string fqn(getFQN(e));

                o % "template < >";
                o % "struct interface< " << fqn 
                    << " >";
                {
                    embrace b(o);
                }
                o << ';';
                *o;
            }

            visit_container(e, *this);
        }

        void visit(StructDef * e)
        {
            const std::string fqn(getFQN(e));
        }
    };
}

bool generator::generate(TranslationUnit_ptr tu)
{
    // Adapted HPP
    {
        output_stream os(std::cout);
        const once o(os, "FILE_ADAPTED_HPP");

        {
            stage1 v(os);
            tu->accept(v);
        }
        {
            stage2 v(os);
            tu->accept(v);
        }
        {
            stage3 v(os);
            tu->accept(v);
        }
    }

    return true;
}

