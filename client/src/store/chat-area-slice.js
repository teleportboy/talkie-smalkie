import { createSlice } from "@reduxjs/toolkit";

const initialState = {
  chats: [
    {
      id: 'chat_1',
      messages: [
        {
          type: 'sent',
          content: '123123'
        },
        {
          type: 'receive',
          content: 'qweqwe'
        },
      ]
    },
  ]
}

export const chatsSlice = createSlice({
  name: 'allChats',
  initialState,
  reducers: {
    createChat: (state, action) => {
      state.chats.push({
        id: action.payload,
        messages: []
      });
    },
    pushMessage: (state, action) => {
      const chat = state.chats.find(chat => {
        return chat.id === action.payload.id;      
      });
      chat.messages.push(action.payload.message);
    },
    setupAllChats: (state, action) => {
      state.chats = action.payload.allChats.map(chat => {
        return {
          id: chat.chatroom,
          messages: chat.messages.map(message => {
            return {
              type: action.payload.uname === 
                message.sender ? 'sent' : 'receive',
              content: message.message
            };
          })
        };
      });
    }
  }
});

export const { pushMessage, createChat, setupAllChats } = chatsSlice.actions;
export default chatsSlice.reducer;