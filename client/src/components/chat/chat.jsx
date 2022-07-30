import React from "react";

import { ChatHeader } from "./header.jsx";
import { MessageArea } from "./message-area.jsx";
import { MessageInput } from "./message-input.jsx";
import { ChatList } from "../all-chats/chat-list.jsx";
import { SearchUser } from "../friends/search-user.jsx";
import { FriendList } from "../friends/friend-list.jsx";

export function Chat() {
  //todo: use navigate
  return (
    <>
      <ChatHeader />
      <MessageArea />
      <MessageInput />
      <ChatList />
      <SearchUser />
      <FriendList />
    </>
  );
}