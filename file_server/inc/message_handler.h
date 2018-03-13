#ifndef _MESSAGE_HANDLER_H
#define _MESSAGE_HANDLER_H

#include "message.h"
#include "db_interface.h"
#include <boost/filesystem.hpp>
#include <fstream>

namespace bitmile{
  class MessageHandler {
  public:
    MessageHandler() {}
    /*! detect types and handle message of each type*/
    msg::Message* Handle (msg::Message* mes);

    msg::Message* HandleKeywordQuery (msg::Message* query);
    msg::Message* HandleUploadDoc (msg::Message* upload_mes);
  private:
    /*!< database interface */
    db::DbInterface db_;

    msg::MessageFactory mes_factory_;
  };
}//namespace bitmile
#endif
