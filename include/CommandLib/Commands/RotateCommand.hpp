#pragma once

#include "../ICommand.hpp"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class RotateCommand : public ICommand {
public:
    explicit RotateCommand(float angle);
    void Execute(IObject* object) override;
    std::vector<uint8_t> Serialize() const override;

    std::string Type() const override;

private:
    float angle_;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int) {
        ar & BOOST_SERIALIZATION_NVP(angle_);
    }
};
