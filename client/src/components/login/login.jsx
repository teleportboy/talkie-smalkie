import React from "react";
import { fetchEventSource } from '@microsoft/fetch-event-source';

export function Login() {
  const onSubmit = (event) => {
    console.log("clicked!");
    event.preventDefault();

    const formData = new FormData(event.target.parentNode);
    const formProps = Object.fromEntries(formData);
    console.log(formProps);
    
    fetchEventSource('/login', {
      method: "POST",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify(formProps),
      onmessage(msg) {
        console.log(msg.data);
      }
    });

  }

  return (
    <form className="login" id="login_form">
      <div className="username-input item">
        <label htmlFor="uname"><b>Username</b></label>
        <input type="text" placeholder="Enter Username" name="uname" required />
      </div>
      <div className="password-input item">
        <label htmlFor="psw"><b>Password</b></label>
        <input type="text" placeholder="Enter Password" name="psw" required />
      </div>
      
      <button type="submit" onClick={onSubmit}>Loooogin</button>
      
      <div className="new-acc-btn item">
        <button type="button">Create new account</button>
      </div>
    </form>
  );
}