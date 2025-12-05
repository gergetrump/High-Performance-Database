//---------------------------------------------------------------------------
// IMLAB
//------------------------------------------------------------
#ifndef IMLAB_ALGEBRA_IU_H
#define IMLAB_ALGEBRA_IU_H
//------------------------------------------------------------
#include "../../imlab/infra/Types.h"

#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>
//------------------------------------------------------------
namespace imlab::algebra {
//------------------------------------------------------------
class IU {
    public:
    /// Column name
    std::string column_;
    /// Data type
    Type type_;

    /// Table IU constructor
    IU(std::string column, Type type) : column_(std::move(column)), type_(type) {}
    /// Destructor
    ~IU() = default;

};
//------------------------------------------------------------
class Scope {
    public:
    struct IUMap {
        std::string alias;
        const IU* iu;
    };
    /// Constructor
    explicit Scope() = default;

    /// Destructor
    ~Scope() = default;

    /// No copy operations
    Scope(const Scope&) = delete;
    Scope& operator=(const Scope&) = delete;

    /// Move operations
    Scope(Scope&&) = default;
    Scope& operator=(Scope&&) = default;



    void addTable(const std::string& tableName, const std::string& alias, const std::vector<const IU*>& columns);

    const IU* resolveIU(const std::string&tableName, const std::string&columnName) const;

    std::vector<const IU*> getAllIUs() const;

    void mergeSubqueryScope(const std::string& alias, const Scope& subqueryScope) {
        for (const auto& [tableAlias, iuMaps] : subqueryScope.ius_) {
            std::string mergedAlias = alias.empty() ? tableAlias : alias + "." + tableAlias;
            ius_[mergedAlias].insert(ius_[mergedAlias].end(), iuMaps.begin(), iuMaps.end());
        }
    }


    std::map<std::string, std::vector<IUMap>> ius_;

    Scope* parent_;

};
//------------------------------------------------------------
} // namespace imlab::algebra
//------------------------------------------------------------
#endif //IMLAB_ALGEBRA_IU_H
