import { configureStore } from '@reduxjs/toolkit';
import chatRoomsReducer from './chatrooms-list-slice.js';
import chatAreasReducer from './chat-area-slice.js';
import currentChatReducer from './current-chat-slice.js';
import authReducer from './auth-slice.js';
import searchChatRoomsReducer from './search-chatrooms-slice.js';

export const store = configureStore({
  reducer: {
    chatRooms: chatRoomsReducer,
    chatAreas: chatAreasReducer,
    currentChat: currentChatReducer,
    auth: authReducer,
    searchChatRooms: searchChatRoomsReducer
  },
});