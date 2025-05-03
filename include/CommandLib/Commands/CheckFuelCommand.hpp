#include <boost/serialization/access.hpp>
#pragma one

#include "../ICommand.hpp"

class CheckFuelCommand : public ICommand {
public:
    void Execute(IObject* object) override;
    std::vector<uint8_t> Serialize() const override;
    std::string Type() const override;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int) {}
};
