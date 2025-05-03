#include <CommandLib/Commands/RotateCommand.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <sstream>

RotateCommand::RotateCommand(float angle) : angle_(angle) {}

void RotateCommand::Execute(IObject* object) {
    if (!object->HasProperty("rotation")) return;

    auto rotData = object->GetProperty("rotation");
    float* rotation = reinterpret_cast<float*>(rotData.data());
    *rotation += angle_;
    object->SetProperty("rotation", std::vector<uint8_t>(
        reinterpret_cast<uint8_t*>(rotation),
        reinterpret_cast<uint8_t*>(rotation + 1)));
}

std::vector<uint8_t> RotateCommand::Serialize() const {
    std::ostringstream oss;
    boost::archive::binary_oarchive oa(oss);
    oa << Type();
    oa << *this;
    return {oss.str().begin(), oss.str().end()};
}

std::string RotateCommand::Type() const {
    return "RotateCommand";
}

namespace {
bool registered = []() -> bool {
    ICommand::RegisterCommand("RotateCommand", [](const uint8_t* data, size_t size, IObject* context) {
        std::istringstream iss(std::string(reinterpret_cast<const char*>(data), size));
        boost::archive::binary_iarchive iar(iss);
        std::string type;
        iar >> type;
        RotateCommand cmd(0.0f);
        iar >> cmd;
        return std::make_shared<RotateCommand>(cmd);
    });
    return true;
}();
}
