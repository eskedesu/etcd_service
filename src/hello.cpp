#include "hello.hpp"

#include <fmt/format.h>

#include <userver/server/handlers/http_handler_base.hpp>

namespace service_template {

namespace {

class Hello final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-hello";

  Hello(const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        etcd_client_ptr_(
            component_context
                .FindComponent<userver::storages::etcd::Component>("etcd")
                .GetClient()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    etcd_client_ptr_->Put("key", "value");
    etcd_client_ptr_->Range("key");
    etcd_client_ptr_->DeleteRange("key");
    return service_template::SayHelloTo(request.GetArg("name"));
  }

 private:
  userver::storages::etcd::ClientPtr etcd_client_ptr_;
};

}  // namespace

std::string SayHelloTo(std::string_view name) {
  if (name.empty()) {
    name = "unknown user";
  }

  return fmt::format("Hello, {}!\n", name);
}

void AppendHello(userver::components::ComponentList& component_list) {
  component_list.Append<Hello>();
}

}  // namespace service_template
