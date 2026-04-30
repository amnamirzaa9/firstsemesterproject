# Clinic Management System (C++)

A comprehensive, console-based application designed to bridge the gap between patients and clinic staff by automating the process of booking and managing medical appointments.

 Project Overview
This system utilizes a database-driven approach to streamline clinic operations through two distinct portals: the **User Portal** and the **Staff Portal**.

---

 Features

 User (Patient) Portal
* **Secure Registration & Login**: Users can create accounts using their CNIC, contact details, and a unique password.
* **Smart Appointment Booking**: Features a real-time list of doctors with their qualifications and fees, preventing double-booking of specific time slots.
* **Token Generation**: Automatically generates a unique Token ID for every confirmed appointment.
* **History Management**: Allows users to view their personal appointment history or cancel visits using their Token ID.

 Staff (Admin) Portal
* **Administrative Control**: Secured by staff credentials to monitor the clinic’s daily schedule.
* **Status Updates**: Enables staff to update appointment statuses (e.g., "Completed") in real-time.
* **Doctor Management**: Staff can add new doctors, set consultation fees, and define available hours.
* **User Oversight**: Provides a high-level view of all registered users and active clinic activity.


 Technical Details
* **Language**: C++
* **Data Persistence**: Uses file handling (`users.txt`, `doctors.txt`, `appointments.txt`) to ensure data is saved and reloaded across sessions.
* **Input Validation**: Includes robust error handling and buffer clearing to prevent crashes from invalid user inputs.
* **Formatted UI**: Utilizes `<iomanip>` and ASCII color codes for a professional-grade dashboard.

