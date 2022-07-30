import React from "react";

export function MessageInput() {
  return (
    <div className="item chat-input input-container">
      <textarea className="input-message" name="message" id="message" defaultValue={""} />
      <button className="send-message">send</button>
      <button className="audio-message">audio</button>
    </div>
  );
}