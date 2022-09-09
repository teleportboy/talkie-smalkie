import React from "react";
import { useSelector } from "react-redux";
import { MessageReceived, MessageSent } from "./message.jsx";

export function MessageArea() {
  const chat = useSelector(state => state.currentChat);

  return (
    <div className="item chat-area">
      {
        chat.messages.map((message, id) => {
          switch (message.type) {
            case 'receive':
              return <MessageReceived content={message.content} key={id}/>;
            case 'sent':
              return <MessageSent content={message.content} key={id}/>;            
          }
        })
      }
    </div>
  );
}