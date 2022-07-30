import React from "react";
import { MessageReceived, MessageSent } from "./message.jsx";

export function MessageArea() {
  return (
    <div className="item chat-area">
      <MessageReceived></MessageReceived>
      <MessageSent></MessageSent>
    </div>
  );
}