import React from "react";

export function MessageReceived({content}) {
  return (
    <div className="message-group-received">
      <div className="message-received">
        <div className="message-received-text">
          {content}
        </div>
      </div>
    </div>
  );
}

export function MessageSent({content}) {
  return (
    <div className="message-group-sent">
      <div className="message-sent">
        <div className="message-sent-text">
          {content}
        </div>
      </div>
    </div>
  );
}