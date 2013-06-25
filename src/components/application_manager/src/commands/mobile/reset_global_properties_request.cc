/*

 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/commands/mobile/reset_global_properties_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_chaining.h"
#include "application_manager/application_impl.h"
#include "JSONHandler/SDLRPCObjects/V2/HMILevel.h"
#include "utils/logger.h"

namespace application_manager {

namespace commands {

log4cxx::LoggerPtr logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Commands"));

ResetGlobalPropertiesRequest::ResetGlobalPropertiesRequest(
    const MessageSharedPtr& message): CommandRequestImpl(message) {
}

ResetGlobalPropertiesRequest::~ResetGlobalPropertiesRequest() {
}

void ResetGlobalPropertiesRequest::Run() {
  LOG4CXX_INFO(logger_, "ResetGlobalPropertiesRequest::Run ");

  ApplicationImpl* app = static_cast<ApplicationImpl*>(
      ApplicationManagerImpl::instance()->
      application((*message_)[strings::params][strings::connection_key]));

  if (NULL == app) {
    LOG4CXX_ERROR_EXT(logger_, "No application associated with session key ");
    SendResponse(false,
                 NsSmartDeviceLinkRPC::V2::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const int correlation_id =
      (*message_)[strings::params][strings::correlation_id];
  const int connection_key =
      (*message_)[strings::params][strings::connection_key];

  size_t obj_length =
      (*message_)[strings::msg_params][strings::properties].length();
  for (size_t i = 0; i < obj_length; ++i) {
    switch ((*message_)[strings::msg_params][strings::properties][i].asInt()) {
      case GlobalProperty::HELPPROMT: {
        ResetHelpPromt(app);
        break;
      }
      case GlobalProperty::TIMEOUTPROMT: {
        ResetTimeoutPromt(app);
        break;
      }
      case GlobalProperty::VRHELPTITLE: {
        ResetVrHelpTitle(app);
        break;
      }
      case GlobalProperty::VRHELPITEMS: {
        ResetVrHelpItems(app);
        break;
      }
      default: {
        LOG4CXX_ERROR(logger_, "Unknown global property 0x%02X value" <<
        (*message_)[strings::msg_params][strings::properties][i].asInt());
        break;
      }
    }
  }

  // there is no request to HMI
  /*
  //TODO(DK): HMI request ID
  const unsigned int cmd_id = 4;

  ApplicationManagerImpl::instance()->AddMessageChain(
      new MessageChaining(connection_key, corellation_id),
      connection_key, corellation_id, cmd_id);

  ApplicationManagerImpl::instance()->SendMessageToHMI(&(*message_));
  */
}

void ResetGlobalPropertiesRequest::ResetHelpPromt(ApplicationImpl* const app,
                                                  bool is_timeout_promp) {
  if (NULL == app) {
    return;
  }

  CommandsMap cmdMap = app->commands_map();
  smart_objects::CSmartObject helpPrompt;

  int index = 0;
  CommandsMap::const_iterator command_it = cmdMap.begin();
  for (; cmdMap.end() != command_it; ++command_it) {
    if (false == (*command_it->second).keyExists(strings::vr_commands)) {
      LOG4CXX_ERROR(logger_, "VR synonyms are empty");
      break;
    }
    // use only first
    helpPrompt[index++] = (*command_it->second)[strings::vr_commands][0];
  }

  if (true == is_timeout_promp) {
    app->set_timeout_prompt(helpPrompt);
  } else {
    app->set_help_prompt(helpPrompt);
  }
}

void ResetGlobalPropertiesRequest::ResetTimeoutPromt(
    ApplicationImpl* const app) {
  ResetHelpPromt(app, true);
}

void ResetGlobalPropertiesRequest::ResetVrHelpTitle(
    ApplicationImpl* const app) {
  if (NULL == app) {
    return;
  }

  smart_objects::CSmartObject help_title(app->name());
  app->set_vr_help_title(help_title);
}

void ResetGlobalPropertiesRequest::ResetVrHelpItems(
    ApplicationImpl* const app) {
}

}  // namespace commands

}  // namespace application_manager