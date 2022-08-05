import React from 'react';
import { Route, Routes } from 'react-router-dom';
import { Chat } from './components/chat/chat.jsx';
import { Login } from './components/login/login.jsx';
import { Registration } from './components/login/registration.jsx';
import './App.css'

export function App() {
  return (
    <div className='container'>
      <Routes>
        <Route path="/" element={<Chat />} />
        <Route path='/login' element={<Login />} />
        <Route path='/registration' element={<Registration />} />
      </Routes>
    </div>
  );
}

{/* <ChatList></ChatList>
<Chat></Chat>
<SearchUser></SearchUser>
<FriendList></FriendList> */}